// Thermocouple sensors
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"

// LMU
#include <Adafruit_LSM6DSOX.h>
#include <data_structs.h>

// // SO2 Sensor
// #include "DFRobot_MultiGasSensor.h"


#define THERMOCOUPLE_I2C_ADDRESS (0x67)

//Enabled by default, use IIC communication at this time. Use UART communication when disabled
#define I2C_COMMUNICATION
// #define SO2_I2C_ADDRESS    0x74
// DFRobot_GAS_I2C gas(&Wire ,SO2_I2C_ADDRESS);

Adafruit_MCP9600 mcp;
Adafruit_LSM6DSOX sox;

void thermo_setup() {

    /* Initialise the driver with THERMOCOUPLE_I2C_ADDRESS and the default I2C bus. */
    if (! mcp.begin(THERMOCOUPLE_I2C_ADDRESS)) {
        Serial.println("\nThermocouple sensor not found. Check wiring!");
    }
  Serial.println("\nThermocouple MCP9600 Found!");

  // Thermocouple setup
  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("\tADC resolution set to ");
  switch (mcp.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

  mcp.setThermocoupleType(MCP9600_TYPE_K);
  Serial.print("\tThermocouple type set to ");
  switch (mcp.getThermocoupleType()) {
    case MCP9600_TYPE_K:  Serial.print("K"); break;
    case MCP9600_TYPE_J:  Serial.print("J"); break;
    case MCP9600_TYPE_T:  Serial.print("T"); break;
    case MCP9600_TYPE_N:  Serial.print("N"); break;
    case MCP9600_TYPE_S:  Serial.print("S"); break;
    case MCP9600_TYPE_E:  Serial.print("E"); break;
    case MCP9600_TYPE_B:  Serial.print("B"); break;
    case MCP9600_TYPE_R:  Serial.print("R"); break;
  }
  Serial.println(" type");

  mcp.setFilterCoefficient(3);
  Serial.print("\tFilter coefficient value set to: ");
  Serial.println(mcp.getFilterCoefficient());

  mcp.setAlertTemperature(1, 30);
  Serial.print("\tAlert #1 temperature set to ");
  Serial.println(mcp.getAlertTemperature(1));
  mcp.configureAlert(1, true, true);  // alert 1 enabled, rising temp

  mcp.enable(true);
  Serial.println("Thermocouple setup done!");
}


void imu_setup() {
  // IMU set up
  // Check for LSM module
  if (!sox.begin_I2C()) {
      Serial.println("\nIMU LSM6DSOX  not initialised");
  }

  Serial.println("\nIMU LSM6DSOX Found!");
    // sox.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  Serial.print("\tAccelerometer range set to: ");
  switch (sox.getAccelRange()) {
  case LSM6DS_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case LSM6DS_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case LSM6DS_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case LSM6DS_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.print("\tGyro range set to: ");
  switch (sox.getGyroRange()) {
  case LSM6DS_GYRO_RANGE_125_DPS:
    Serial.println("125 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  case ISM330DHCX_GYRO_RANGE_4000_DPS:
    break; // unsupported range for the DSOX
  }

  // sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("\tAccelerometer data rate set to: ");
  switch (sox.getAccelDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  // sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("\tGyro data rate set to: ");
  switch (sox.getGyroDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }
  Serial.println("IMU setup done!");
}

thermo_union thermo() {
  // Thermocouple Temperature
  Serial.print("Hot Junction: "); Serial.println(mcp.readThermocouple()); // float
  Serial.print("Cold Junction: "); Serial.println(mcp.readAmbient());     // float
  Serial.print("ADC: "); Serial.print(mcp.readADC() * 2); Serial.println(" uV");  // int32_t

  thermo_union thermo_readings;
  thermo_readings.fl[0] = mcp.readThermocouple();
  thermo_readings.fl[1] = mcp.readAmbient();
  thermo_readings.fl[2] = (float) mcp.readADC();
  return thermo_readings;
}

imu_union imu()
{
  // IMU Readings
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);

  Serial.println("");
  Serial.print("Temperature ");
  Serial.print(temp.temperature);  // float
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("Accel X: ");
  Serial.print(accel.acceleration.x); // float
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y); // float
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z); // float
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  Serial.print("Gyro X: ");
  Serial.print(gyro.gyro.x); // float
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y); // float
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z); // float
  Serial.println(" radians/s ");
  Serial.println();

  imu_union imu_readings;
  imu_readings.fl[0] = temp.temperature;
  imu_readings.fl[1] = accel.acceleration.x;
  imu_readings.fl[2] = accel.acceleration.y;
  imu_readings.fl[3] = accel.acceleration.z;
  imu_readings.fl[4] = gyro.gyro.x;
  imu_readings.fl[5] = gyro.gyro.y;
  imu_readings.fl[6] = gyro.gyro.z;
  return imu_readings; 
}