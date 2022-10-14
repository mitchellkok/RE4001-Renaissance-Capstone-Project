#include <SPI.h>             

#define VSPI_MISO 19
#define VSPI_MOSI 23
#define VSPI_SCLK 18
#define VSPI_CS   5              

void setup (void)
{
  Serial.begin(9600);

  SPI.begin();                            //Begins the SPI commnuication
  SPI.setClockDivider(SPI_CLOCK_DIV8);    //Sets clock for SPI communication at 1 MHz
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);                 
}

//Hello, World! in hex
char cbuf[1];
byte buf[14] = {0x48,0x65,0x6c,0x6c,0x6f,0x2c,0x20,0x57,0x6f,0x72,0x6c,0x64,0x21,0x00};

void loop(void)
{
  
  for(int i = 0; i < 14; i++){
    digitalWrite(SS, LOW);
    SPI.transfer(buf[i]); 
    digitalWrite(SS, HIGH);

    // sprintf(cbuf, buf);
    char s[4];
    snprintf(s, 4, "%c", buf[i]);
    Serial.print(buf[i], HEX);
    Serial.print(" ");
    Serial.println(s);
  }
 
  Serial.println();
  delay(5000);
}