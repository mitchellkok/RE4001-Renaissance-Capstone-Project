#include <HardwareSerial.h>
#include <data_structs.h>

HardwareSerial SerialPort(2); // use UART2

void ecsense_so2_setup() {
  SerialPort.begin(9600, SERIAL_8N1, 16, 17);
  delay(10);
  // const unsigned char commandOne[9] = {0XFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47};
  // SerialPort.write(commandOne, 9);
  // delay(10);
  const unsigned char commandTwo[9] = {0XFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};
  SerialPort.write(commandTwo, 9);  // Passive
  delay(10);
  Serial.println("ECSense SO2 setup done!");
}

ecsense_so2_union ecsense_so2_reading() {
  // const unsigned char message[1] = {0xD1};
  // SerialPort.write(message, 1);

  const unsigned char commandFive[9] = {0XFF, 01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  SerialPort.write(commandFive, 9);

  // delay(50);
  // Read each of the 9 bytes
  int start_index = -1;
  float gasValue = -1;
  uint8_t output[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  int available = SerialPort.available();
  if (available >= 9) {
    Serial.println("");
    Serial.print("ECSENSE SO2:");
    Serial.println(available);
    bool start_capture = false;
    uint8_t prev_val = 0;
    for(int n=0; n<available; n++) {
        uint8_t val = SerialPort.read();
        if (n>0 && val == 0x86 && prev_val == 0xFF) {
          start_capture = true;
          start_index = n-1;
          Serial.println("****");
        }
        if (start_capture == true) {
          output[n] = val;
          Serial.print("*");
        }
        Serial.print(n);Serial.print(": ");Serial.print(val, HEX); Serial.println("");
        if (n == (start_index + 9)){ 
          start_capture = false;
          // break; 
        }
        prev_val = val;
    }

    if (start_index > -1) {
      gasValue = (float)((output[start_index+6] * 256 + output[start_index+7])) / 100;
    }
    Serial.print(gasValue);
    Serial.println(" ppm");
  } 

  ecsense_so2_union so2_readings;
  for(int n=0; n<9; n++) { so2_readings.readings.full_reading[n] = output[n];}
  so2_readings.readings.ecsense_so2 = gasValue;
  return so2_readings;
}

ecsense_so2_union ecsense_so2_reading_full() {

  const unsigned char commandSix[9] = {0XFF, 00, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  SerialPort.write(commandSix, 9);

  int start_index = 0;
  float gasValue = -1;
  float temp = -1;
  float humidity = -1;
  uint8_t output[13] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  if (SerialPort.available() >= 13) {
    Serial.println("");
    Serial.println("ECSENSE SO2:");
    // for(int n=0; n<26; n++) {
    //     output[n] = SerialPort.read();
    //     Serial.print(output[n], HEX);
    //     Serial.print(" ");
    //     if (output[n] == 0x86 && output[n-1] == 0xFF) {
    //       start_index = n-1;
    //     }
    //     if (n == (start_index + 8)){
    //       break;
    //     }
    // }
    for(int n=0; n<13; n++) {
        output[n] = SerialPort.read();
        Serial.print(output[n], HEX);
        Serial.print(" ");
    }
    Serial.println("");
    gasValue = (float)((output[start_index+6] * 256 + output[start_index+7])) / 100;
    temp = (float)((output[start_index+8] << 8 ) | (output[start_index+9])) / 100;
    humidity = (float)((output[start_index+10] << 8 ) | (output[start_index+11])) / 100;
    Serial.print(gasValue);Serial.println(" ppm");
    Serial.print(temp);Serial.println(" deg C");
    Serial.print(humidity);Serial.println(" rh%");
  } 

  ecsense_so2_union so2_readings;
  for(int n=0; n<13; n++) { so2_readings.readings.full_reading[n] = output[n]; }
  so2_readings.readings.ecsense_so2 = gasValue;
  so2_readings.readings.ecsense_temp = temp;
  so2_readings.readings.ecsense_hum = humidity;
  return so2_readings;
}