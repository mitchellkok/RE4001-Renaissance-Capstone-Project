#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // use UART2
unsigned char output[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void china_so2_setup() {
  SerialPort.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("CHINA SO2 setup done!");
}

void china_so2_reading() {
  const unsigned char message[1] = {0xD1};
  const unsigned char commandFive[9] = {0XFF, 01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  // SerialPort.write(message, 1);
  SerialPort.write(commandFive, 9);

  // Read each of the 9 bytes
  if (SerialPort.available() >= 9) {
    Serial.println("");
    Serial.println("CHINA SO2:");
    for(int n=0; n<9; n++) {
        output[n] = SerialPort.read();
        Serial.print(output[n], HEX);
        Serial.print(" ");
    }
    Serial.println("");
    float gasValue = (float)((output[6] * 256 + output[7])) / 100;
    Serial.println(gasValue);
    Serial.println("");
    delay(1000);
  }  
}