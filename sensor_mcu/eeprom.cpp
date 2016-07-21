/*
 * EEPROM
 * 
 * Assumes that the Sensor Data block doesn't start at 0x0000
 */

#include "eeprom.h"
#include "Arduino.h"

#include <Wire.h>

unsigned int getModuloInOffsetRange(unsigned int start, unsigned int end, unsigned int target) {
  return (target - start) % (end - start + 1) + start; 
}

// will search the buffer for the first non zero number

unsigned int readTriplicatedWordFromBuffer(unsigned int buffer_start, unsigned int buffer_end, unsigned int default_value, unsigned int min_value, unsigned int max_value) {
  byte *value_1_temp;
  byte *value_2_temp;
  byte *value_3_temp;
  int value_1;
  int value_2;
  int value_3;

  for( unsigned int i = buffer_start; i <= buffer_end; i += 2 ) {
    value_1_temp = readEEPROM(disk1, getModuloInOffsetRange(buffer_start, buffer_end, i), 2, buffer_start, buffer_end);
    value_1 = value_1_temp[1] | value_1_temp[0] << 8;
    free(value_1_temp);

    Serial.println("Address:");
    Serial.println(i);
    Serial.println("Value:");
    Serial.println(value_1);
    
    if( value_1 == 0 ) {
      continue;
    }

    value_2_temp = readEEPROM(disk1, getModuloInOffsetRange(buffer_start, buffer_end, i + 2), 2, buffer_start, buffer_end);
    value_3_temp = readEEPROM(disk1, getModuloInOffsetRange(buffer_start, buffer_end, i + 4), 2, buffer_start, buffer_end);
    value_2 = value_2_temp[1] | value_2_temp[0] << 8;
    value_3 = value_3_temp[1] | value_3_temp[0] << 8;
    free(value_2_temp);
    free(value_3_temp);
    
    
    if( (value_1 == value_2 || value_1 == value_3) && value_1 >= min_value && value_1 <= max_value ) {
      return value_1;
    } else if( (value_2 == value_3) && value_2 >= min_value && value_2 <= max_value ) {
      return value_2;
    }
  }

  return default_value;
}

void writeTriplicatedWordToBufferAtNextLocation(unsigned int data, unsigned int buffer_start, unsigned int buffer_end, unsigned int default_value, unsigned int min_value, unsigned int max_value) {
  byte *value_temp;
  unsigned int value;
  unsigned int address;
  
  for( unsigned int i = buffer_start; i <= buffer_end; i += 2 ) {
    address = getModuloInOffsetRange(buffer_start, buffer_end, i);
    value_temp = readEEPROM(disk1, address, 2, buffer_start, buffer_end);
    value = value_temp[1] | value_temp[0] << 8;
    free(value_temp);

    if( value != 0 ) {
      // put the head in triplicate in the 6 bytes after the ones we are about to erase
      // erase the 6 bytes at the address we are looking at
      for( unsigned int j = 0; j < 3; j++ ) {
        _writeByteEEPROM(disk1, address + 6 + (2 * j), (byte)(data & 0xFF00 >> 8));
        _writeByteEEPROM(disk1, address + 6 + (2 * j + 1), (byte)(data & 0x00FF));
      }
      for( unsigned int j = 0; j < 6; j++ ) {
        _writeByteEEPROM(disk1, address + j, 0);
      }

      delay(5);

      break;
    }
  }
}

// HEAD_BUFFER_START - HEAD_BUFFER_END
unsigned int readHead() {
  return readTriplicatedWordFromBuffer(HEAD_BUFFER_START, HEAD_BUFFER_END, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_END);
}

// HEAD_BUFFER_START - HEAD_BUFFER_END
void writeHead(unsigned int head) {
  writeTriplicatedWordToBufferAtNextLocation(head, HEAD_BUFFER_START, HEAD_BUFFER_END, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_END);
}

// TAIL_BUFFER_START - TAIL_BUFFER_END
unsigned int readTail() {
  return readTriplicatedWordFromBuffer(TAIL_BUFFER_START, TAIL_BUFFER_END, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_END);
}

// TAIL_BUFFER_START - TAIL_BUFFER_END
void writeTail(unsigned int tail) {
    writeTriplicatedWordToBufferAtNextLocation(tail, TAIL_BUFFER_START, TAIL_BUFFER_END, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_END);
}

// SENSOR_DATA_BUFFER_START - SENSOR_DATA_BUFFER_END
SensorData readSensorData() {
  // TODO
}

// SENSOR_DATA_BUFFER_START - SENSOR_DATA_BUFFER_END
void writeSensorData(byte *data, unsigned int data_length) {
  unsigned int sensor_data_address = readHead();
  writeEEPROM(disk1, sensor_data_address, data, data_length);
  writeHead(getModuloInOffsetRange(SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_END, sensor_data_address + data_length));
}

byte _readByteEEPROM(unsigned int deviceaddress, unsigned int eeaddress) {
  byte rdata = 0xFF;
 
  Wire.beginTransmission((int)deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress, 1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}

void _writeByteEEPROM(unsigned int deviceaddress, unsigned int eeaddress, byte data) {
  Wire.beginTransmission((int)deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
}

byte readByteEEPROM(unsigned int deviceaddress, unsigned int eeaddress) {
  return _readByteEEPROM(deviceaddress, eeaddress);
}

void writeByteEEPROM(unsigned int deviceaddress, unsigned int eeaddress, byte data) {
  _writeByteEEPROM(deviceaddress, eeaddress, data);

  Serial.println("Writing to EEPROM");
  Serial.println(eeaddress);
  Serial.println(data);
 
  delay(5);
}

byte *readEEPROM(unsigned int deviceaddress, unsigned int eeaddress, unsigned int num_bytes, unsigned int buffer_start, unsigned int buffer_end) {
  byte *data = (byte*)malloc(num_bytes * sizeof(byte));
  int buffer_size = buffer_end - buffer_start + 1;
  
  if( !(eeaddress >= buffer_start && eeaddress <= buffer_end) ) {
    Serial.println("Bad eeaddress");
    return 0x00;
  }
  if( !(buffer_start < buffer_end - 0x01 && buffer_start <= EEPROM_LAST_ADDRESS && buffer_end <= EEPROM_LAST_ADDRESS) ) {
    Serial.println("Bad buffer constraints");
    return 0x00;
  }

  for( unsigned int i = 0; i < num_bytes; i++ ) {
    int address = eeaddress + i;
    int normalized_address = address - buffer_start;
    int target_address = normalized_address % buffer_size + buffer_start;
    data[i] = _readByteEEPROM(deviceaddress, target_address);
  }

  return data;
}

void writeEEPROM(unsigned int deviceaddress, unsigned int eeaddress, byte *data, unsigned int data_length) {
  //http://stackoverflow.com/questions/36104244/c-how-to-do-struct-packing-and-unpacking-into-char-buffer

  for( unsigned int i = 0; i < data_length; i++ ) {
    _writeByteEEPROM(deviceaddress, eeaddress, data[i]);
  }

  delay(5);
}

