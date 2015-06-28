/*
 * EEPROM Clear
 *
 * Sets all of the bytes of the EEPROM to 0.
 * This example code is in the public domain.

 */

#include <EEPROM.h>

void setup()
{
  Serial.begin(115200);
  delay(5000);
  Serial.print("EEPROM Clear Start");
  EEPROM.begin(512);
  // write a 0xFF to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0xFF);

  // turn the LED on when we're done
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  EEPROM.end();
  Serial.print("EEPROM Clear End");
}

void loop()
{
}
