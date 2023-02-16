#include "DFRobot_MultiGasSensor.h"
#include <data_structs.h>

//Enabled by default, use IIC communication at this time. Use UART communication when disabled
#define I2C_COMMUNICATION
#define SO2_I2C_ADDRESS    0x74
DFRobot_GAS_I2C gas(&Wire ,SO2_I2C_ADDRESS);

void gravity_so2_setup() {

  while(!gas.begin())
  {
    Serial.println("NO Gas Sensor Devices !");
    delay(1000);
    break;
  }
  gas.changeAcquireMode(gas.PASSIVITY);
  delay(1000);
  gas.setTempCompensation(gas.OFF);
  Serial.println("Gravity SO2 setup done!");
}

gravity_so2_union gravity_so2() {
  Serial.println("\nReading Gravity SO2");
  Serial.print("Ambient ");
  Serial.print(gas.queryGasType());
  Serial.print(" concentration is: ");
  Serial.print(gas.readGasConcentrationPPM());  // float
  Serial.println(" ppm");
  Serial.print("The board temperature is: ");
  Serial.print(gas.readTempC());  // float
  Serial.println(" ℃");
  Serial.println();

  gravity_so2_union so2_readings;
  so2_readings.fl[0] = gas.readGasConcentrationPPM();
  so2_readings.fl[1] = gas.readTempC();
  return so2_readings;
}