#include <Arduino.h>
#include <SPI.h>     

#include <gps_atm.h>
#include <imu_thermo.h>

// #define VSPI_MISO 19
// #define VSPI_MOSI 23
// #define VSPI_SCLK 18
// #define VSPI_CS   5     
#define BUF_LEN 14  
#define READINGS_DELAY_MS 2000
#define READINGS_INTERVAL_MS 5000

union data_union {
    char s[BUF_LEN];
    uint8_t buf[BUF_LEN];
};
data_union tx_union;
data_union rx_union;

byte command[BUF_LEN]     = {0x48,0x65,0x6c,0x6c,0x6f,0x2c,0x20,0x57,0x6f,0x72,0x6c,0x64,0x21,0x00};
byte trigger_cmd[BUF_LEN];
byte request_cmd[BUF_LEN];

void print_rxtx(data_union rx, data_union tx) {
  // Master does TX then RX
  Serial.print("TX BUFFER: ");
  for(int i=0;i<BUF_LEN;i++){
    Serial.print(tx.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("RX BUFFER: ");
  for(int i=0;i<BUF_LEN;i++){
    Serial.print(rx.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

void spi_setup() {
  SPI.begin();                            //Begins the SPI commnuication
  SPI.setClockDivider(SPI_CLOCK_DIV8);    //Sets clock for SPI communication at 1 MHz
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  trigger_cmd[0] = 0xAA;
  request_cmd[0] = 0xBB;
}

void buffer_setup(data_union rx, data_union tx, bool print) {
  memset(tx.buf, 0, BUF_LEN); // clear buffers
  memset(rx.buf, 0, BUF_LEN);
  memcpy(tx.buf, command, BUF_LEN); //copy test command to buffer
  if (print) {print_rxtx(rx, tx);}
}

void spi_rxtx(byte* cmd, data_union rx, data_union tx){
  memcpy(tx.buf, cmd, BUF_LEN); // load in command to trigger readings

  digitalWrite(SS, LOW);  // pull chip select low
  SPI.transferBytes(tx.buf, rx.buf, BUF_LEN);
  digitalWrite(SS, HIGH);

  print_rxtx(rx, tx);
  Serial.println();
}

void setup (void)
{
  Serial.begin(9600);
  spi_setup();
  buffer_setup(rx_union, tx_union, true);  

  gpsSerial.begin(GPSBaud);
  init3in1();

  imu_thermo_setup();
  Serial.println("Ready\n");             
}

void loop(void)
{
  GPS();
  imu_thermo();

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