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

    // TODO: Change to this format
    // jsonDocument["type"] = tag;
    // jsonDocument["value"] = value;

    jsonDocument["date_year"] = rx->data_struct.datetime.datetime.year;
    jsonDocument["date_month"] = rx->data_struct.datetime.datetime.b[0];
    jsonDocument["date_day"] = rx->data_struct.datetime.datetime.b[1];
    jsonDocument["date_hour"] = rx->data_struct.datetime.datetime.b[2];
    jsonDocument["date_min"] = rx->data_struct.datetime.datetime.b[3];
    jsonDocument["date_sec"] = rx->data_struct.datetime.datetime.b[4];

    jsonDocument["gravity_so2conc"] = rx->data_struct.gravity_so2.fl[0];
    jsonDocument["gravity_so2temp"] = rx->data_struct.gravity_so2.fl[1];

    jsonDocument["atm_master_temp"] = rx->data_struct.atm_master.fl[0];
    jsonDocument["atm_master_pressure"] = rx->data_struct.atm_master.fl[1];
    jsonDocument["atm_master_humidity"] = rx->data_struct.atm_master.fl[2];

    jsonDocument["atm_slave_temp"] = rx->data_struct.atm_slave.fl[0];
    jsonDocument["atm_slave_pressure"] = rx->data_struct.atm_slave.fl[1];
    jsonDocument["atm_slave_humidity"] = rx->data_struct.atm_slave.fl[2];

    serializeJson(jsonDocument, buffer);
    return buffer;
}

void setup_http() {     
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(SSID);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
} 