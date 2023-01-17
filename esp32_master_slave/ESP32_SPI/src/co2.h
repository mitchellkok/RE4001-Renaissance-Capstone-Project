// CO2 Meter K-series Example Interface
// Revised by Marv Kausch, 7/2016 at CO2 Meter <co2meter.com>
// Talks via I2C to K30/K22/K33/Logger sensors and displays CO2 values
// 12/31/09
#include <Wire.h>
// We will be using the I2C hardware interface on the Arduino in
// combination with the built-in Wire library to interface.
// Arduino analog input 5 - I2C SCL
// Arduino analog input 4 - I2C SDA
/*
  In this example we will do a basic read of the CO2 value and checksum verification.
  For more advanced applications please see the I2C Comm guide.
*/

#define LED 2
#define I2C_SDA 21
#define I2C_SCL 22

int co2Addr = 0x68;

///////////////////////////////////////////////////////////////////
// Function : int readCO2()
// Returns : CO2 Value upon success, 0 upon checksum failure
// Assumes : - Wire library has been imported successfully.
// - LED is connected to IO pin 13
// - CO2 sensor address is defined in co2_addr
///////////////////////////////////////////////////////////////////
int readCO2()
{
  int co2_value = 0;  // We will store the CO2 value inside this variable.

  digitalWrite(LED, HIGH);  // turn on LED
  // On most Arduino platforms this pin is used as an indicator light.

  //////////////////////////
  /* Begin Write Sequence */
  //////////////////////////

  Wire.beginTransmission(co2Addr);
  Wire.write(0x22);
  Wire.write(0x00);
  Wire.write(0x08);
  Wire.write(0x2A);

  Wire.endTransmission();

  /////////////////////////
  /* End Write Sequence. */
  /////////////////////////

  /*
    We wait 10ms for the sensor to process our command.
    The sensors's primary duties are to accurately
    measure CO2 values. Waiting 10ms will ensure the
    data is properly written to RAM

  */

  delay(10);

  /////////////////////////
  /* Begin Read Sequence */
  /////////////////////////

  /*
    Since we requested 2 bytes from the sensor we must
    read in 4 bytes. This includes the payload, checksum,
    and command status byte.

  */

  Wire.requestFrom(co2Addr, 4);

  byte i = 0;
  byte buffer[4] = {0, 0, 0, 0};

  /*
    Wire.available() is not nessessary. Implementation is obscure but we leave
    it in here for portability and to future proof our code
  */
  while (Wire.available())
  {
    buffer[i] = Wire.read();
    i++;
  }

  ///////////////////////
  /* End Read Sequence */
  ///////////////////////

  /*
    Using some bitwise manipulation we will shift our buffer
    into an integer for general consumption
  */

  co2_value = 0;
  co2_value |= buffer[1] & 0xFF;
  co2_value = co2_value << 8;
  co2_value |= buffer[2] & 0xFF;


  byte sum = 0; //Checksum Byte
  sum = buffer[0] + buffer[1] + buffer[2]; //Byte addition utilizes overflow

  if (sum == buffer[3])
  {
    // Success!
    digitalWrite(LED, LOW);
    return co2_value;
  }
  else
  {
    // Failure!
    /*
      Checksum failure can be due to a number of factors,
      fuzzy electrons, sensor busy, etc.
    */

    digitalWrite(LED, LOW);
    return 0;
  }
}

void i2c_scan() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}

// This is the default address of the CO2 sensor, 7bits shifted left.
void co2_setup() {
//   Serial.begin(9600);
//   Wire.begin(I2C_SDA, I2C_SCL);
  pinMode(LED, OUTPUT); // address of the Arduino LED indicator
  i2c_scan();
  Serial.println("");
  Serial.println("CO2 Application Note AN-102: Interface Arduino to K-30");
}


void co2_reading() {

  int co2Value = readCO2();
  // Serial.println(co2Value);
  if (co2Value > 0)
  {
    Serial.print("CO2 Value: ");
    Serial.println(co2Value);
  }
  else
  {
    Serial.println("Checksum failed / Communication failure");
  }
  delay(2000);
}

