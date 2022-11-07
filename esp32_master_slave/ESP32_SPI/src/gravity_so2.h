#include "DFRobot_MultiGasSensor.h"

//Enabled by default, use IIC communication at this time. Use UART communication when disabled
#define I2C_COMMUNICATION
#define SO2_I2C_ADDRESS    0x74
DFRobot_GAS_I2C gas(&Wire ,SO2_I2C_ADDRESS);

union gravity_so2_union {
    float fl[2]; // float size is 4 Bytes
    uint8_t buf[8];
};

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

gravity_so2_union gravity_so2() {
  Serial.println("\nReading Gravity SO2");
  Serial.print("Ambient ");
  Serial.print(gas.queryGasType());
  Serial.print(" concentration is: ");
  Serial.print(gas.readGasConcentrationPPM());  // float
  Serial.println(" %vol");
  Serial.print("The board temperature is: ");
  Serial.print(gas.readTempC());  // float
  Serial.println(" ℃");
  Serial.println();

  gravity_so2_union so2_readings;
  so2_readings.fl[0] = gas.readGasConcentrationPPM();
  so2_readings.fl[1] = gas.readTempC();
  return so2_readings;
}