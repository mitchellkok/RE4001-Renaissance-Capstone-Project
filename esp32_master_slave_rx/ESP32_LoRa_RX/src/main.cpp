#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <data_structs.h>
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
  Serial.begin(9600);
  delay(10);

  Serial.println("Setting up WiFi and HTTP");
  setup_http();
  server.on("/getdata", HTTP_GET, [](AsyncWebServerRequest *request) {
    char* buffer = getData(&rx);
    request->send(200, "application/json", buffer);
  });
  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis and set reconnect delay to 1 second
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
      rx.data_struct.rx_rssi = rf95.lastRssi();
      
      Serial.print("Received LEN = ");
      Serial.println(sizeof(rx.buf));
      print_lora_union(rx); 
      Serial.println("Sending ping");

      sse_getData(&rx);
      
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