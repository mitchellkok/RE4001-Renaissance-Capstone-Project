#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // use UART2
unsigned char output[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void ecsense_so2_setup() {
  SerialPort.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("ECSENSE SO2 setup done!");
}

float ecsense_so2_reading() {
  // const unsigned char message[1] = {0xD1};
  // SerialPort.write(message, 1);

  const unsigned char commandFive[9] = {0XFF, 01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  SerialPort.write(commandFive, 9);

  // Read each of the 9 bytes
  int start_index = 0;
  if (SerialPort.available() >= 9) {
    Serial.println("");
    Serial.println("ECSENSE SO2:");
    for(int n=0; n<18; n++) {
        output[n] = SerialPort.read();
        Serial.print(output[n], HEX);
        Serial.print(" ");
        if (output[n] == 0x86 && output[n-1] == 0xFF) {
          start_index = n-1;
        }
        if (n == (start_index + 8)){
          break;
        }
    }
    Serial.println("");
    // float gasValue = (float)((output[6] * 256 + output[7])) / 100;
    float gasValue = (float)((output[start_index+6] * 256 + output[start_index+7])) / 100;
    Serial.println(gasValue);
    Serial.println(" ppm");
    return gasValue;
  }  
  return -1;
}