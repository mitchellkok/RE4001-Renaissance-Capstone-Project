// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include <SPI.h>
#include <RH_RF95.h>

  /* ESP32 feather w/wing */
  #define RFM95_RST     26  // "A"
  #define RFM95_CS      25  // "B"
  #define RFM95_INT     14  //  next to A


// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 433.0

// union lora_union_tx {
//     uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; // 251 bytes
// };

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void lora_setup() 
{
  // cli();  // stop interrupts
  pinMode(RFM95_RST, OUTPUT);
  pinMode(RFM95_INT, INPUT_PULLDOWN);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(20);
  digitalWrite(RFM95_RST, HIGH);
  delay(20);

  Serial.println("Feather LoRa TX Test!");
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    delay(2000);
  }
  pinMode(RFM95_CS, OUTPUT);
  digitalWrite(RFM95_CS, HIGH);
  Serial.println("LoRa radio init OK!");
  // sei(); // resume interrupts

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

int16_t lora(uint8_t* tx)
{
  Serial.print("LoRa TX Sending... ");
  Serial.print(RH_RF95_MAX_MESSAGE_LEN); Serial.println(" Bytes");
  rf95.send(tx, RH_RF95_MAX_MESSAGE_LEN);

  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent(2000);  // 2000ms timeout
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply...");
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }
  return rf95.lastRssi();
}
