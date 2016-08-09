
// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MCP3425
// This code is designed to work with the MCP3425_I2CADC I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Analog-Digital-Converters?sku=MCP3425_I2CADC#tabs-0-product_tabset-2

#include <math.h>
#include<Wire.h>

// MCP3425 I2C address is 0x68(104)
#define Addr 0x68

void setup_12bit_data();

void setup() 
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Start serial communication and set baud rate = 9600
  Serial.begin(9600);
  

  //setup_12bit_data();
  
  delay(300);
}

void setup_16bit_data_with_8xgain() {
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Send configuration command
  // Continuous conversion mode, 12-bit resolution
  Wire.write(0x8B);  // 10001011 Initiate_read-XX-One_shot-16_bit-8xPGA_gain; 10001000 Initiate_read-XX-One_shot-16_bit-1xPGA_gain
  // Stop I2C Transmission
  Wire.endTransmission();

  delay(300);
}

void get_16bit_data_with_8xgain() {
  unsigned int data[2];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb 
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }


  Serial.print("Raw data: ");
  Serial.print(data[0]);
  Serial.print(' ');
  Serial.println(data[1]);
  // Convert the data to 16-bits
  int raw_adc = (data[0] & 0xFF) * 256 + data[1];
  if(raw_adc > pow(2, 16) / 2.0)
  {
    raw_adc -= pow(2, 16);
  }

  double processed_adc = double(raw_adc) * 62.5 / 1000000.0 / 8.0;
  
  // Output data to serial monitor
  Serial.print("Digital Value of Analog Input: ");
  Serial.println(raw_adc);
  
  Serial.print("Voltage Based on Digital Value: ");
  Serial.println(processed_adc, 5);
  delay(1000);
}

void setup_16bit_data() {
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Send configuration command
  // Continuous conversion mode, 12-bit resolution
  Wire.write(0x88);  // 10001011 Initiate_read-XX-One_shot-16_bit-8xPGA_gain; 10001000 Initiate_read-XX-One_shot-16_bit-1xPGA_gain
  // Stop I2C Transmission
  Wire.endTransmission();

  delay(300);
}

void get_16bit_data() {
  unsigned int data[2];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb 
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }


  Serial.print("Raw data: ");
  Serial.print(data[0]);
  Serial.print(' ');
  Serial.println(data[1]);
  // Convert the data to 16-bits
  int raw_adc = (data[0] & 0xFF) * 256 + data[1];
  if(raw_adc > pow(2, 16) / 2.0)
  {
    raw_adc -= pow(2, 16);
  }

  double processed_adc = double(raw_adc) * 62.5 / 1000000.0;
  
  // Output data to serial monitor
  Serial.print("Digital Value of Analog Input: ");
  Serial.println(raw_adc);
  
  Serial.print("Voltage Based on Digital Value: ");
  Serial.println(processed_adc, 5);
  delay(1000);
}

void setup_12bit_data() {
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Send configuration command
  // Continuous conversion mode, 12-bit resolution
  Wire.write(0x10);
  // Stop I2C Transmission
  Wire.endTransmission();
}

void get_12bit_data() {
  unsigned int data[2];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb 
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
   
  // Convert the data to 12-bits
  int raw_adc = (data[0] & 0x0F) * 256 + data[1];
  if(raw_adc > 2047)
  {
    raw_adc -= 4096;
  }
  
  // Output data to serial monitor
  Serial.print("Digital Value of Analog Input : ");
  Serial.println(raw_adc);
  delay(500);
}

void loop() {
  setup_16bit_data_with_8xgain();
  get_16bit_data_with_8xgain();
  //setup_16bit_data();
  //get_16bit_data();
  //get_12bit_data();
}
