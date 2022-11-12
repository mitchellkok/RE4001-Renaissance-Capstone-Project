#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <data_structs.h>
// #include <mqtt.h>
#include <http.h>

/* ESP32 feather w/wing */
#define RFM95_RST     27   // "A"
#define RFM95_CS      33   // "B"
#define RFM95_INT     12   //  next to A

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 433.0

// Blinky on receipt
#define LED 13

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
lora_union rx;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  delay(100);

  Serial.println("Setting up WiFi and HTTP");
  setup_http();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("AP Accessed!!!");
  });
  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "TESTING TESTING");
  });
  server.on("/getdata", HTTP_GET, [](AsyncWebServerRequest *request) {
    char* buffer = getData(&rx);
    request->send(200, "application/json", buffer);
  });
  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();  

  Serial.println("Feather LoRa RX Test!");
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now
    // uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(rx.buf);

    if (rf95.recv(rx.buf, &len))
    {
      digitalWrite(LED, HIGH);
      Serial.print("\nRSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      Serial.print("Received LEN = ");
      Serial.println(sizeof(rx.buf));
      print_lora_union(rx); 
      events.send("ping",NULL,millis());
      
      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
//*/

// publish mqtt
      // if (!client.connected()) {
      //   Serial.println("Publishing MQTT...");
      //   char str[80];

      //   // datetime
      //   sprintf(str, "%u", rx.data_struct.datetime.datetime.year);client.publish("esp32/date_year", str);
      //   sprintf(str, "%u", rx.data_struct.datetime.datetime.b[0]);client.publish("esp32/date_month", str);
      //   sprintf(str, "%u", rx.data_struct.datetime.datetime.b[1]);client.publish("esp32/date_day", str);
      //   sprintf(str, "%u", rx.data_struct.datetime.datetime.b[2]);client.publish("esp32/date_hour", str);
      //   sprintf(str, "%u", rx.data_struct.datetime.datetime.b[3]);client.publish("esp32/date_min", str);
      //   sprintf(str, "%u", rx.data_struct.datetime.datetime.b[4]);client.publish("esp32/date_sec", str);

      //   // gravity SO2
      //   sprintf(str, "%f", rx.data_struct.gravity_so2.fl[0]);client.publish("esp32/gravity_so2conc", str);
      //   sprintf(str, "%f", rx.data_struct.gravity_so2.fl[1]);client.publish("esp32/gravity_so2temp", str);

      //   // atm master
      //   sprintf(str, "%f", rx.data_struct.atm_master.fl[0]);client.publish("esp32/atm_master_temp", str);
      //   sprintf(str, "%f", rx.data_struct.atm_master.fl[1]);client.publish("esp32/atm_master_pressure", str);
      //   sprintf(str, "%f", rx.data_struct.atm_master.fl[2]);client.publish("esp32/atm_master_humidity", str);
        
      //   // atm slave
      //   sprintf(str, "%f", rx.data_struct.atm_slave.fl[0]);client.publish("esp32/atm_slave_temp", str);
      //   sprintf(str, "%f", rx.data_struct.atm_slave.fl[1]);client.publish("esp32/atm_slave_pressure", str);
      //   sprintf(str, "%f", rx.data_struct.atm_slave.fl[2]);client.publish("esp32/atm_slave_humidity", str);
        
      //   // gps
      //   sprintf(str, "%u", rx.data_struct.gps_slave.readings.satellites);client.publish("esp32/gps_satellites", str);
      //   sprintf(str, "%f", rx.data_struct.gps_slave.readings.lat);client.publish("esp32/gps_lat", str);
      //   sprintf(str, "%f", rx.data_struct.gps_slave.readings.lng);client.publish("esp32/gps_lng", str);
      //   sprintf(str, "%f", rx.data_struct.gps_slave.readings.meters);client.publish("esp32/gps_meters", str);
      //   sprintf(str, "%f", rx.data_struct.gps_slave.readings.deg);client.publish("esp32/gps_deg", str);
      //   sprintf(str, "%f", rx.data_struct.gps_slave.readings.mps);client.publish("esp32/gps_mps", str);
      //   sprintf(str, "%u", rx.data_struct.gps_slave.readings.hdop);client.publish("esp32/gps_hdop", str);

      //   // imu
      //   sprintf(str, "%f", rx.data_struct.imu.fl[0]);client.publish("esp32/imu_temp", str);
      //   sprintf(str, "%f", rx.data_struct.imu.fl[1]);client.publish("esp32/imu_acc_x", str);
      //   sprintf(str, "%f", rx.data_struct.imu.fl[2]);client.publish("esp32/imu_acc_y", str);
      //   sprintf(str, "%f", rx.data_struct.imu.fl[3]);client.publish("esp32/imu_acc_z", str);
      //   sprintf(str, "%f", rx.data_struct.imu.fl[4]);client.publish("esp32/imu_gyro_x", str);
      //   sprintf(str, "%f", rx.data_struct.imu.fl[5]);client.publish("esp32/imu_gyro_y", str);
      //   sprintf(str, "%f", rx.data_struct.imu.fl[6]);client.publish("esp32/imu_gyro_z", str);

      //   // thermocouple
      //   sprintf(str, "%f", rx.data_struct.thermocouple.fl[0]);client.publish("esp32/thermo_thermo", str);
      //   sprintf(str, "%f", rx.data_struct.thermocouple.fl[1]);client.publish("esp32/thermo_ambient", str);
      //   sprintf(str, "%f", rx.data_struct.thermocouple.fl[2]);client.publish("esp32/thermo_adc", str);
      // } 