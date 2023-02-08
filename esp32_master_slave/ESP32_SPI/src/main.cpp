#include <Arduino.h>

#include <spi_master.h>
#include <atm.h>
#include <imu_thermo.h>
#include <gravity_so2.h>
#include <ecsense_so2.h>
#include <lora_tx.h>
#include <sd_rtc.h>
#include <data_structs.h>
#include <batt.h>

#define SDCARD  1
#define LORA    1
#define SLAVE   1

#define STARTBYTE 0xAA
#define Y_LED 4
#define I2C_SDA 21
#define I2C_SCL 22

data_union tx_union;
data_union rx_union;
int16_t last_rssi = 0;

void setup (void)
{
  pinMode(Y_LED, OUTPUT); // Declare the LED as an output
  digitalWrite(Y_LED, HIGH); // Turn the LED on
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);

  spi_setup();
  buffer_setup(&rx_union, &tx_union, true);  

  #ifdef SDCARD
    sd_rtc_setup();
  #endif
  
  init3in1();
  gravity_so2_setup();
  ecsense_so2_setup();
  imu_setup();
  thermo_setup();

  #ifdef LORA
    lora_setup();
  #endif
 
  digitalWrite(Y_LED, LOW); // Turn the LED off      
  Serial.println("Master Ready\n");       
}

void loop(void)
{
  #ifdef SDCARD
    Serial.println("HERE0");datetime_union datetime = get_datetime();
  #endif
  Serial.println("HERE1");float batt_voltage = get_battery_voltage();
  Serial.println("HERE2");float batt_percent = get_battery_percent(batt_voltage);
  Serial.println("HERE3");imu_union imu_readings = imu();
  Serial.println("HERE4");thermo_union thermo_readings = thermo();
  Serial.println("HERE5");atm_union atm_master = dispAtmData();
  Serial.println("HERE6");gravity_so2_union gravity_so2_readings = gravity_so2();
  Serial.println("HERE7");float ecsense_so2 = ecsense_so2_reading();
  Serial.println("HERE8");
  #ifdef SLAVE
    cli();  // stop interrupts
    trigger_cmd[1]++; // counter to track trigger number
    Serial.println("");Serial.printf("SPI Master Command Sent: 0x%x (%d)", trigger_cmd[0], trigger_cmd[0]);Serial.println("");
    spi_rxtx(trigger_cmd, &rx_union, &tx_union); // Send command to trigger readings (Command == 0xAA)
    delay(READINGS_DELAY_MS); // delay 2000ms to wait for slave to take readings

    request_cmd[1] = trigger_cmd[1];
    Serial.printf("SPI Master Command Sent: 0x%x (%d)", request_cmd[0], request_cmd[0]);Serial.println("");
    spi_rxtx(request_cmd, &rx_union, &tx_union); // Send command to request readings (Command == 0xBB)

    atm_union atm_slave = rx_union.readings.atm;
    gps_union gps_slave = rx_union.readings.gps;
    int co2_slave = rx_union.readings.co2;
    // TODO: SLAVE BATTERY READINGS
    
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
    Serial.println("");
    Serial.println("Slave CO2 Readings:");
    Serial.print(co2_slave);Serial.println(" PPM");
    // TODO: read, check checksum (IF MISMATCH, request immediately)
    sei(); // resume interrupts
  #endif

  // LoRa
  Serial.println("");
  lora_union tx;
  tx.data_struct.start_byte = STARTBYTE;
  #ifdef SDCARD
    tx.data_struct.datetime = datetime;
  #endif
  tx.data_struct.gravity_so2 = gravity_so2_readings;
  tx.data_struct.atm_master = atm_master;
  tx.data_struct.imu = imu_readings;
  tx.data_struct.thermocouple = thermo_readings;
  tx.data_struct.ecsense_so2 = ecsense_so2;
  #ifdef SLAVE
    tx.data_struct.atm_slave = atm_slave;
    tx.data_struct.gps_slave = gps_slave;
    tx.data_struct.co2 = co2_slave;
  #endif
  tx.data_struct.m_battery_voltage = batt_voltage;
  tx.data_struct.m_battry_percent = batt_percent;
  tx.data_struct.tx_rssi = last_rssi;
  #ifdef LORA
    last_rssi = lora(tx.buf);
  #endif 

  Serial.println("");
  delay(2000);

  #ifdef SDCARD
    sd_save_data();
  #endif
  delay(READINGS_INTERVAL_MS - READINGS_DELAY_MS); // remaining delay before triggering readings
}