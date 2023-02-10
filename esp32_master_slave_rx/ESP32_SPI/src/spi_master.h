#include <Arduino.h>
#include <SPI.h>     
#include <data_structs.h>

// #define VSPI_MISO 19
// #define VSPI_MOSI 23
// #define VSPI_SCLK 18
#define VSPI_CS  0     
#define BUF_LEN 64
#define SLAVE_READINGS_DELAY_MS 2000

byte command[BUF_LEN]     = {0x48,0x65,0x6c,0x6c,0x6f,0x2c,0x20,0x57,0x6f,0x72,0x6c,0x64,0x21,0x00};
byte trigger_cmd[BUF_LEN];
byte request_cmd[BUF_LEN];

void print_rxtx(data_union rx, data_union tx) {
  // Master does TX then RX
  Serial.println("TX BUFFER: ");
  for(int i=0;i<BUF_LEN;i++){
    Serial.print(tx.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("RX BUFFER: ");
  for(int i=0;i<BUF_LEN;i++){
    Serial.print(rx.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

void spi_setup() {
  SPI.begin();                            //Begins the SPI commnuication
  SPI.setClockDivider(SPI_CLOCK_DIV8);    //Sets clock for SPI communication at 1 MHz
  pinMode(VSPI_CS, OUTPUT);
  digitalWrite(VSPI_CS, HIGH);

  trigger_cmd[0] = 0xAA;
  request_cmd[0] = 0xBB;
}

void buffer_setup(data_union* rx, data_union* tx, bool print) {
  memset((*tx).buf, 0, BUF_LEN); // clear buffers
  memset((*rx).buf, 0, BUF_LEN);
  memcpy((*tx).buf, command, BUF_LEN); //copy test command to buffer
  if (print) {print_rxtx((*rx), (*tx));}
}

void spi_rxtx(byte* cmd, data_union *rx, data_union *tx){
  // cli(); //stop interrupts 
  memcpy((*tx).buf, cmd, BUF_LEN); // load in command to trigger readings

  digitalWrite(VSPI_CS, LOW);  // pull chip select low
  SPI.transferBytes((*tx).buf, (*rx).buf, BUF_LEN);
  digitalWrite(VSPI_CS, HIGH);

  print_rxtx((*rx), (*tx));
  Serial.println();
  // sei(); //resume interrupts 
}