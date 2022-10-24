#include "DFRobot_MultiGasSensor.h"

//Enabled by default, use IIC communication at this time. Use UART communication when disabled
#define I2C_COMMUNICATION
#define SO2_I2C_ADDRESS    0x74
DFRobot_GAS_I2C gas(&Wire ,SO2_I2C_ADDRESS);


void gravity_so2_setup() {

  while(!gas.begin())
  {
    Serial.println("NO Gas Sensor Devices !");
    delay(1000);
  }
  Serial.println("The device is connected successfully!");

  gas.changeAcquireMode(gas.PASSIVITY);
  delay(1000);

  gas.setTempCompensation(gas.OFF);
}

void gravity_so2() {

  Serial.print("Ambient ");
  Serial.print(gas.queryGasType());
  Serial.print(" concentration is: ");
  Serial.print(gas.readGasConcentrationPPM());
  Serial.println(" %vol");
  Serial.print("The board temperature is: ");
  Serial.print(gas.readTempC());
  Serial.println(" ℃");
  Serial.println();
}