// CO2 Meter K-series Example Interface
// Revised by Marv Kausch, 7/2016 at CO2 Meter <co2meter.com>
// Talks via I2C to K30/K22/K33/Logger sensors and displays CO2 values
// 12/31/09
#include <Wire.h>
/*
  In this example we will do a basic read of the CO2 value and checksum verification.
  For more advanced applications please see the I2C Comm guide.
*/

int co2Addr = 0x68;

// Function : int readCO2()
// Returns : CO2 Value upon success, 0 upon checksum failure
// Assumes :  - Wire library has been imported successfully.
//            - CO2 sensor address is defined in co2_addr
int readCO2()
{
  int co2_value = 0;  // We will store the CO2 value inside this variable.

  Wire.beginTransmission(co2Addr);
  Wire.write(0x22);
  Wire.write(0x00);
  Wire.write(0x08);
  Wire.write(0x2A);
  Wire.endTransmission();

  // We wait 10ms for the sensor to process our command. The sensors's primary duties are to accurately
  // measure CO2 values. Waiting 10ms will ensure the data is properly written to RAM
  delay(10);

  // Since we requested 2 bytes from the sensor we must
  // read in 4 bytes. This includes the payload, checksum,
  // and command status byte.
  Wire.requestFrom(co2Addr, 4);

  byte i = 0;
  byte buffer[4] = {0, 0, 0, 0};

  // Wire.available() is not nessessary. Implementation is obscure but we leave
  // it in here for portability and to future proof our code
  while (Wire.available()) {
    buffer[i] = Wire.read();
    i++;
  }

  // Using some bitwise manipulation we will shift our buffer into an integer for general consumption
  co2_value = 0;
  co2_value |= buffer[1] & 0xFF;
  co2_value = co2_value << 8;
  co2_value |= buffer[2] & 0xFF;

  byte sum = 0; //Checksum Byte
  sum = buffer[0] + buffer[1] + buffer[2]; //Byte addition utilizes overflow

  if (sum == buffer[3]) {
    // Success!
    return co2_value;
  } else {
    // Failure! Checksum failure can be due to a number of factors, fuzzy electrons, sensor busy, etc.
    return 0;
  }
}

void co2_reading() {
  int co2Value = readCO2();
  if (co2Value > 0) {
    Serial.print("CO2 Value: ");
    Serial.println(co2Value);
  } else {
    Serial.println("CO2 Checksum failed / Communication failure");
  }
  Serial.println("");
}

