#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // use UART2
unsigned char output[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() {
  SerialPort.begin(9600, SERIAL_8N1, 16, 17);
}

void loop() {
  const unsigned char message[1] = {0xD1};
  const unsigned char commandFive[9] = {0XFF, 01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  // SerialPort.write(message, 1);
  SerialPort.write(commandFive, 9);

  // Read each of the 9 bytes
  if (SerialPort.available() >= 9) {
    for(int n=0; n<9; n++) {
    output[n] = SerialPort.read();
    Serial.print(output[n], HEX);
    Serial.print(" ");
    }
    Serial.println("");
    delay(1000);
  }

  int gasValue = output[6] * 256 + output[7];
  Serial.println(gasValue);
}