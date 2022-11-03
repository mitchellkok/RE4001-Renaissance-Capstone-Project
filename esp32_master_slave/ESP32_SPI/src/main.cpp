#include <Arduino.h>

#include <spi_master.h>
#include <atm.h>
#include <imu_thermo.h>
#include <gravity_so2.h>
#include <lora_tx.h>
#include <sd_rtc.h>

data_union tx_union;
data_union rx_union;

void setup (void)
{
  Serial.begin(9600);
  spi_setup();
  buffer_setup(rx_union, tx_union, true);  
  sd_rtc_setup();
  

  // gpsSerial.begin(GPSBaud);
  init3in1();
  gravity_so2_setup();
  imu_thermo_setup();
  lora_setup();

  Serial.println("Ready\n");             
}

void loop(void)
{
  // GPS();
  imu_thermo();
  dispAtmData();

  Serial.println("\nReading Gravity SO2");
  gravity_so2();

  lora();

  cli();
  trigger_cmd[1]++; // counter to track trigger number
  Serial.println("");Serial.printf("SPI Master Command Sent: %d", trigger_cmd[0]);Serial.println("");
  spi_rxtx(trigger_cmd, &rx_union, &tx_union); // Send command to trigger readings (Command == 0xAA)
  delay(READINGS_DELAY_MS); // delay 2000ms to wait for slave to take readings

  request_cmd[1] = trigger_cmd[1];
  Serial.printf("SPI Master Command Sent: %d", request_cmd[0]);Serial.println("");
  spi_rxtx(request_cmd, &rx_union, &tx_union); // Send command to request readings (Command == 0xBB)

  atm_union atm_readings;
  for (int i = 0; i < 12; i++) {atm_readings.buf[i] = rx_union.buf[i];}  // load atm readings into buffer
  
  Serial.println("Slave Readings:");
  for (int i = 0; i < 3; i++) {
      Serial.printf("Reading %d: %f", i, atm_readings.fl[i]);
      Serial.println("");
  }
  // TODO: read, check checksum (IF MISMATCH, request immediately)
  sei();
  
  Serial.println("");
  delay(2000);
  sd_save_data();
  delay(READINGS_INTERVAL_MS - READINGS_DELAY_MS); // remaining delay before triggering readings
}