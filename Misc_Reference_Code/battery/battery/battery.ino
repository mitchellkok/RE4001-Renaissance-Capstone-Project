#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <MovingAverageFloat.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
MovingAverageFloat <8> readings;

#define VBAT_PIN 35
#define BATTV_MAX    4.1     // maximum voltage of battery
#define BATTV_MIN    3.2     // what we regard as an empty battery
#define BATTV_LOW    3.4     // voltage considered to be low battery
#define ADC_STEPS    4095
#define SCALER       0.96
#define LED 4

float get_battery_voltage() {
    float battv = ((float)analogRead(VBAT_PIN) / ADC_STEPS) * BATTV_MAX * SCALER;
    if (battv <= BATTV_MIN) {
        digitalWrite(LED, HIGH); // Turn the LED on
    }
    return battv;
}

float get_battery_percent() {
    readings.add(get_battery_voltage());
    float battv = readings.get();
    float battpc = (((battv - BATTV_MIN) / (BATTV_MAX - BATTV_MIN)) * 100);

    lcd.clear();         
    lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
    lcd.print(battv);
    lcd.print("V");
    lcd.setCursor(2,1);   //Set cursor to character 2 on line 1
    lcd.print(battpc);
    lcd.print("%");
    return battpc;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT); // Declare the LED as an output
  digitalWrite(LED, HIGH); // Turn the LED on
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  digitalWrite(LED, LOW); // Turn the LED on
}

void loop() {
  // put your main code here, to run repeatedly:
  // get_battery_percent();
  // delay(2000);
  scan();
}


void scan()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
