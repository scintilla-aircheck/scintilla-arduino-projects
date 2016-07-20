#include <Wire.h>    
 
#define SERIAL_BAUD_RATE 9600

#define disk1 0x50    //Address of 24LC256 eeprom chip
#define EEPROM_FIRST_ADDRESS 0x0000
#define EEPROM_LAST_ADDRESS 0x7FFF
#define HEAD_BUFFER_START 0x0000
#define HEAD_BUFFER_END 0x00FF
#define TAIL_BUFFER_START 0x0100
#define TAIL_BUFFER_END 0x01FF
#define SENSOR_DATA_BUFFER_START 0x0200
#define SENSOR_DATA_BUFFER_END EEPROM_LAST_ADDRESS

struct SensorData {
  
};

int readHead();
void writeHead(unsigned int head);
int readTail();
void writeTail(unsigned int tail);
SensorData readSensorData();
void writeSensorData(SensorData data);
byte _readByteEEPROM(int deviceaddress, unsigned int eeaddress);
void _writeByteEEPROM(int deviceaddress, unsigned int eeaddress, byte data);
byte readByteEEPROM(int deviceaddress, unsigned int eeaddress);
void writeByteEEPROM(int deviceaddress, unsigned int eeaddress, byte data);
byte *readEEPROM(int deviceaddress, unsigned int eeaddress, unsigned int num_bytes, unsigned int buffer_start, unsigned int buffer_end);
void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte *data);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Serial connected.");

  Wire.begin();

  for( unsigned int i = HEAD_BUFFER_START; i <= HEAD_BUFFER_END; i++ ) {
    writeByteEEPROM(disk1, i, 4);
  }

  writeByteEEPROM(disk1, 16, 69);
  writeByteEEPROM(disk1, 17, 69);
  writeByteEEPROM(disk1, 18, 69);
  writeByteEEPROM(disk1, 19, 69);
  writeByteEEPROM(disk1, 20, 69);
  writeByteEEPROM(disk1, 21, 69);


  Serial.println(readByteEEPROM(disk1, 0));
  Serial.println(readByteEEPROM(disk1, 1));
  Serial.println(readByteEEPROM(disk1, 2));
  Serial.println(readByteEEPROM(disk1, 3));
  Serial.println(readByteEEPROM(disk1, 16));
  Serial.println(readByteEEPROM(disk1, 17));
  Serial.println(readByteEEPROM(disk1, 18));
  Serial.println(readByteEEPROM(disk1, 19));

  

  Serial.println("Setup complete!");
}

void loop() { // run over and over

}
/*
int readTriplicatedWordFromBuffer(unsigned int buffer_start, unsigned int buffer_end, unsigned int default_value, unsigned int min_value, unsigned int max_value) {
  byte *value_1_temp;
  byte *value_2_temp;
  byte *value_3_temp;
  int value_1;
  int value_2;
  int value_3;

  for( unsigned int i = buffer_start; i <= buffer_end; i += 2 ) {
    value_1_temp = readEEPROM(disk1, (i - buffer_start) % (buffer_end - buffer_start + 1) + buffer_start, 2, buffer_start, buffer_end);
    value_2_temp = readEEPROM(disk1, (i - buffer_start + 0x0002) % (buffer_end - buffer_start + 1) + buffer_start, 2, buffer_start, buffer_end);
    value_3_temp = readEEPROM(disk1, (i - buffer_start + 0x0004) % (buffer_end - buffer_start + 1) + buffer_start, 2, buffer_start, buffer_end);
    value_1 = value_1_temp[1] | value_1_temp[0] << 8;
    value_2 = value_2_temp[1] | value_2_temp[0] << 8;
    value_3 = value_3_temp[1] | value_3_temp[0] << 8;
    free(value_1_temp);
    free(value_2_temp);
    free(value_3_temp);
    Serial.println("Address:");
    Serial.println(i);
    Serial.println("Value:");
    Serial.println(value_1);
    if( (value_1 == value_2 || value_1 == value_3) && value_1 >= min_value && value_1 <= max_value ) {
      return value_1;
    } else if( (value_2 == value_3) && value_2 >= min_value && value_2 <= max_value ) {
      return value_2;
    }
  }

  return default_value;
}
*/

int readTriplicatedWordFromBuffer(unsigned int buffer_start, unsigned int buffer_end, unsigned int default_value, unsigned int min_value, unsigned int max_value) {
  byte *value_1_temp;
  byte *value_2_temp;
  byte *value_3_temp;
  int value_1;
  int value_2;
  int value_3;

  for( unsigned int i = buffer_start; i <= buffer_end; i += 2 ) {
    value_1_temp = readEEPROM(disk1, (i - buffer_start) % (buffer_end - buffer_start + 1) + buffer_start, 2, buffer_start, buffer_end);
    value_1 = value_1_temp[1] | value_1_temp[0] << 8;
    free(value_1_temp);

    Serial.println("Address:");
    Serial.println(i);
    Serial.println("Value:");
    Serial.println(value_1);
    
    if( value_1 == 0 ) {
      continue;
    }

    value_2_temp = readEEPROM(disk1, (i - buffer_start + 0x0002) % (buffer_end - buffer_start + 1) + buffer_start, 2, buffer_start, buffer_end);
    value_3_temp = readEEPROM(disk1, (i - buffer_start + 0x0004) % (buffer_end - buffer_start + 1) + buffer_start, 2, buffer_start, buffer_end);
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

// HEAD_BUFFER_START - HEAD_BUFFER_END
int readHead() {
  return readTriplicatedWordFromBuffer(HEAD_BUFFER_START, HEAD_BUFFER_END, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_END);
}

// HEAD_BUFFER_START - HEAD_BUFFER_END
void writeHead(unsigned int head) {
  //writeByteEEPROM(disk1, address, 123);
}

// TAIL_BUFFER_START - TAIL_BUFFER_END
int readTail() {
  return readTriplicatedWordFromBuffer(TAIL_BUFFER_START, TAIL_BUFFER_END, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_START, SENSOR_DATA_BUFFER_END);
}

// TAIL_BUFFER_START - TAIL_BUFFER_END
void writeTail(unsigned int tail) {
  
}

// SENSOR_DATA_BUFFER_START - SENSOR_DATA_BUFFER_END
SensorData readSensorData() {
  
}

// SENSOR_DATA_BUFFER_START - SENSOR_DATA_BUFFER_END
void writeSensorData(SensorData data) {
  
}

byte _readByteEEPROM(int deviceaddress, unsigned int eeaddress) {
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress, 1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}

void _writeByteEEPROM(int deviceaddress, unsigned int eeaddress, byte data) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
}

byte readByteEEPROM(int deviceaddress, unsigned int eeaddress) {
  return _readByteEEPROM(deviceaddress, eeaddress);
}

void writeByteEEPROM(int deviceaddress, unsigned int eeaddress, byte data) {
  _writeByteEEPROM(deviceaddress, eeaddress, data);

  Serial.println("Writing to EEPROM");
  Serial.println(eeaddress);
  Serial.println(data);
 
  delay(5);
}

byte *readEEPROM(int deviceaddress, unsigned int eeaddress, unsigned int num_bytes, unsigned int buffer_start, unsigned int buffer_end) {
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

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte *data, unsigned int data_length) {
  //http://stackoverflow.com/questions/36104244/c-how-to-do-struct-packing-and-unpacking-into-char-buffer
}

