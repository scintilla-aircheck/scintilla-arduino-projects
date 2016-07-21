#include <stdio.h>
#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>
#include "sensor_data.pb.h"
#include "eeprom.h"

SensorDataMessage sensor_data_message = SensorDataMessage_init_zero;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Serial connected.");
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors(sensor_data_message);
  Serial.print("Lucky Number: ");
  Serial.println(sensor_data_message.lucky_number);

  writeSensorDataToStorage(sensor_data_message);

  delay(1000);
}

SensorDataMessage readSensors(SensorDataMessage &message) {
  message.lucky_number += 1;
}

int writeSensorDataToStorage(SensorDataMessage &message) {
  byte buffer[1024];  // TODO change the buffer size to accomodate the sensor data
  size_t message_length;
  bool status;

  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

  /* Now we are ready to encode the message! */
  status = pb_encode(&stream, SensorDataMessage_fields, &message);
  message_length = stream.bytes_written;
    
  /* Then just check for any errors.. */
  if( !status ) {
      Serial.print("Encoding failed: ");
      Serial.println(PB_GET_ERROR(&stream));
      return 1;
  }

  Serial.print("Message Length: ");
  Serial.println(message_length);
  Serial.print("Buffer: ");
  Serial.println((char*)(buffer));

  writeSensorData((byte*)buffer, message_length);
}

// send all unsent sensor data to the api
void sendSensorDataToAPI() {
  
}

