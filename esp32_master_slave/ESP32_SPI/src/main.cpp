#include <Arduino.h>

#include <spi_master.h>
#include <atm.h>
#include <imu_thermo.h>
#include <gravity_so2.h>
#include <lora_tx.h>
#include <sd_rtc.h>
#include <data_structs.h>

# define STARTBYTE 0xAA

data_union tx_union;
data_union rx_union;

void setup (void)
{
  Serial.begin(9600);
  spi_setup();
  buffer_setup(&rx_union, &tx_union, true);  
  sd_rtc_setup();

  init3in1();
  gravity_so2_setup();
  imu_thermo_setup();
  lora_setup();

  Serial.println("Ready\n");             
}

void loop(void)
{
  datetime_union datetime = get_datetime();
  imu_union imu_readings = imu();
  thermo_union thermo_readings = thermo();
  atm_union atm_master = dispAtmData();
  gravity_so2_union gravity_so2_readings = gravity_so2();

  cli();
  trigger_cmd[1]++; // counter to track trigger number
  Serial.println("");Serial.printf("SPI Master Command Sent: %d", trigger_cmd[0]);Serial.println("");
  spi_rxtx(trigger_cmd, &rx_union, &tx_union); // Send command to trigger readings (Command == 0xAA)
  delay(READINGS_DELAY_MS); // delay 2000ms to wait for slave to take readings

  request_cmd[1] = trigger_cmd[1];
  Serial.printf("SPI Master Command Sent: %d", request_cmd[0]);Serial.println("");
  spi_rxtx(request_cmd, &rx_union, &tx_union); // Send command to request readings (Command == 0xBB)

  atm_union atm_slave = rx_union.readings.atm;
  gps_union gps_slave = rx_union.readings.gps;
  
  Serial.println("Slave 3 in 1 Readings:");
  Serial.printf("Temperature: %f", atm_slave.fl[0]);Serial.println("");
  Serial.printf("Pressure: %f", atm_slave.fl[1]);Serial.println("");
  Serial.printf("Humidity: %f", atm_slave.fl[2]);Serial.println("");
  Serial.println("");
  Serial.println("Slave GPS Readings:");
  Serial.printf("Satellites: %d", gps_slave.readings.satellites);Serial.println("");
  Serial.printf("Latitude: %f", gps_slave.readings.lat);Serial.println("");
  Serial.printf("Longitude: %f", gps_slave.readings.lng);Serial.println("");
  Serial.printf("Altitude: %f", gps_slave.readings.meters);Serial.println("");
  Serial.printf("Degrees: %f", gps_slave.readings.deg);Serial.println("");
  Serial.printf("MPS: %f", gps_slave.readings.mps);Serial.println("");
  Serial.printf("HDOP: %d", gps_slave.readings.hdop);Serial.println("");
  // TODO: read, check checksum (IF MISMATCH, request immediately)
  sei();

  // LoRa
  Serial.println("");
  lora_union tx;
  tx.data_struct.start_byte = STARTBYTE;
  tx.data_struct.datetime = datetime;
  tx.data_struct.gravity_so2 = gravity_so2_readings;
  tx.data_struct.atm_master = atm_master;
  tx.data_struct.atm_slave = atm_slave;
  tx.data_struct.gps_slave = gps_slave;
  tx.data_struct.imu = imu_readings;
  tx.data_struct.thermocouple = thermo_readings;
  lora(tx.buf);
  
  Serial.println("");
  delay(2000);
  sd_save_data();
  delay(READINGS_INTERVAL_MS - READINGS_DELAY_MS); // remaining delay before triggering readings
}