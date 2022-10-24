#include <Arduino.h>

#include <spi_master.h>
#include <atm.h>
#include <imu_thermo.h>
#include <gravity_so2.h>

data_union tx_union;
data_union rx_union;

void setup (void)
{
  Serial.begin(9600);
  spi_setup();
  buffer_setup(rx_union, tx_union, true);  

  // gpsSerial.begin(GPSBaud);
  init3in1();
  gravity_so2_setup();
  // imu_thermo_setup();
  Serial.println("Ready\n");             
}

void loop(void)
{
  // GPS();
  // imu_thermo();
  dispAtmData();

  Serial.println("\nReading Gravity SO2");
  gravity_so2();

  trigger_cmd[1]++; // counter to track trigger number
  Serial.printf("SPI Master Command Sent: %d", trigger_cmd[0]);Serial.println("");
  spi_rxtx(trigger_cmd, rx_union, tx_union); // Send command to trigger readings (Command == 0xAA)
  delay(READINGS_DELAY_MS); // delay 2000ms to wait for slave to take readings

  request_cmd[1] = trigger_cmd[1];
  Serial.printf("SPI Master Command Sent: %d", request_cmd[0]);Serial.println("");
  spi_rxtx(request_cmd, rx_union, tx_union); // Send command to request readings (Command == 0xBB)
  // TODO: read, check checksum (IF MISMATCH, request immediately)
  
  Serial.println("");
  delay(READINGS_INTERVAL_MS - READINGS_DELAY_MS); // remaining delay before triggering readings
}