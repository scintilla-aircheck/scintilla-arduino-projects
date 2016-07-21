#include "Arduino.h"

#ifndef EEPROM
#define EEPROM

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

unsigned int readTriplicatedWordFromBuffer(unsigned int buffer_start, unsigned int buffer_end, unsigned int default_value, unsigned int min_value, unsigned int max_value);
unsigned int readHead();
void writeHead(unsigned int head);
unsigned int readTail();
void writeTail(unsigned int tail);
SensorData readSensorData();
void writeSensorData(byte *data, unsigned int data_length);
byte _readByteEEPROM(unsigned int deviceaddress, unsigned int eeaddress);
void _writeByteEEPROM(unsigned int deviceaddress, unsigned int eeaddress, byte data);
byte readByteEEPROM(unsigned int deviceaddress, unsigned int eeaddress);
void writeByteEEPROM(unsigned int deviceaddress, unsigned int eeaddress, byte data);
byte *readEEPROM(unsigned int deviceaddress, unsigned int eeaddress, unsigned int num_bytes, unsigned int buffer_start, unsigned int buffer_end);
void writeEEPROM(unsigned int deviceaddress, unsigned int eeaddress, byte *data, unsigned int data_length);

#endif
