#include <Wire.h>
#include <Teensy_MCP23017.h>
#include <Teensy_RGBLCDShield.h>

// LCD Stuff

// The shield uses the I2C SCL and SDA pins. 
// You can connect other I2C sensors to the I2C bus and share
// the I2C bus.

Teensy_RGBLCDShield lcd = Teensy_RGBLCDShield(7);


// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

const uint8_t color[] = { RED, GREEN, YELLOW, BLUE, VIOLET, TEAL, WHITE };

uint8_t const lcdChars = 20;
uint8_t const lcdRows  = 4;

char lcdStr[lcdChars + 1];
char versionBuf[lcdChars + 1];

// End LCD Stuff

const long baudRate = 115200;
uint32_t cnt = 0;

void setup()
{
  Wire.begin();
  Serial.begin(baudRate);
  
  delay(10000);
  
  Serial.print(F("Serial Debug starting at "));
  Serial.print(baudRate);
  
  Serial.println(F("Setup LCD"));
  lcd.begin(lcdChars, lcdRows);
  lcd.clear();
  lcd.home();
  lcd.print(F("Serial Debug = "));
  lcd.print(baudRate);
  lcd.setCursor(0, 1);
  lcd.print(F("LCD # 1"));
  lcd.setCursor(0, 2);
  lcd.print(F("LCD Address = 0x27"));
  Serial.println(F("Setup LCD complete"));
}

void loop()
{
  Serial.println(F("Start LCD count"));
  lcd.setCursor(0, 3);
  lcd.print(F("Count = 0x"));
  lcd.print(cnt, HEX);
  lcd.setBacklight(WHITE);
  Serial.println(F("Stop LCD count"));
  cnt++;
  delay(500);
}

