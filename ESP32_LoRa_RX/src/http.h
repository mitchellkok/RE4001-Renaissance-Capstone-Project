#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <data_structs.h>

const char *SSID = "ESP Basestation";
const char *PWD = "Rcapstone";

AsyncWebServer server(80);
AsyncEventSource events("/events");
 
StaticJsonDocument<500> jsonDocument;
char buffer[500];

void setup_http() {     
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(SSID);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
} 
 
void create_json(char *tag, float value) {  
  jsonDocument.clear();  
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  serializeJson(jsonDocument, buffer);
}
 
void add_json_object(char *tag, float value) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
}

char* getData(lora_union *rx) {
    Serial.println("###### getting data ######");
    jsonDocument.clear();

    jsonDocument["date_year"] = rx->data_struct.datetime.datetime.year;
    jsonDocument["date_month"] = rx->data_struct.datetime.datetime.b[0];
    jsonDocument["date_day"] = rx->data_struct.datetime.datetime.b[1];
    jsonDocument["date_hour"] = rx->data_struct.datetime.datetime.b[2];
    jsonDocument["date_min"] = rx->data_struct.datetime.datetime.b[3];
    jsonDocument["date_sec"] = rx->data_struct.datetime.datetime.b[4];

    jsonDocument["m_t"] = rx->data_struct.atm_master.fl[0];
    jsonDocument["m_p"] = rx->data_struct.atm_master.fl[1];
    jsonDocument["m_h"] = rx->data_struct.atm_master.fl[2];

    jsonDocument["s_t"] = rx->data_struct.atm_slave.fl[0];
    jsonDocument["s_p"] = rx->data_struct.atm_slave.fl[1];
    jsonDocument["s_h"] = rx->data_struct.atm_slave.fl[2];

    jsonDocument["g_so2"] = rx->data_struct.gravity_so2.fl[0];
    jsonDocument["g_t"] = rx->data_struct.gravity_so2.fl[1];

    jsonDocument["t_t"] = rx->data_struct.thermocouple.fl[0];

    jsonDocument["e_so2"] = rx->data_struct.ecsense_so2;

    jsonDocument["co2"] = rx->data_struct.co2;

    jsonDocument["tx_rssi"] = rx->data_struct.tx_rssi;
    jsonDocument["rx_rssi"] = rx->data_struct.rx_rssi;

    jsonDocument["gps_sat"] = rx->data_struct.gps_slave.readings.satellites;
    jsonDocument["gps_hdop"] = rx->data_struct.gps_slave.readings.hdop;
    jsonDocument["gps_lat"] = rx->data_struct.gps_slave.readings.lat;
    jsonDocument["gps_lng"] = rx->data_struct.gps_slave.readings.lng;
    jsonDocument["gps_alt"] = rx->data_struct.gps_slave.readings.meters;
    // jsonDocument["gps_deg"] = rx->data_struct.gps_slave.readings.deg;
    jsonDocument["gps_mps"] = rx->data_struct.gps_slave.readings.mps;
    jsonDocument["gps_val"] = rx->data_struct.gps_slave.readings.value;

    jsonDocument["imu_tmp"] = rx->data_struct.imu.fl[0];
    jsonDocument["imu_acx"] = rx->data_struct.imu.fl[1];
    jsonDocument["imu_acy"] = rx->data_struct.imu.fl[2];
    jsonDocument["imu_acz"] = rx->data_struct.imu.fl[3];
    jsonDocument["imu_gyx"] = rx->data_struct.imu.fl[4];
    jsonDocument["imu_gyy"] = rx->data_struct.imu.fl[5];
    jsonDocument["imu_gyz"] = rx->data_struct.imu.fl[6];

    jsonDocument["m_bv"] = rx->data_struct.m_battery_voltage;
    jsonDocument["m_bp"] = rx->data_struct.m_battry_percent;
    jsonDocument["s_bv"] = rx->data_struct.s_battery_voltage;
    jsonDocument["s_bp"] = rx->data_struct.s_battry_percent;

    serializeJson(jsonDocument, buffer);
    return buffer;
}

void sse_getData(lora_union *rx) {
    char* buffer = getData(rx);
    events.send(buffer,NULL,millis());
}
 