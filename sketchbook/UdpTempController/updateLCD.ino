/*
UdpTempController - updateLCD.ino

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.0
This software uses multiple libraries that are subject to additional
licenses as defined by the author of that software. It is the user's
and developer's responsibility to determine and adhere to any additional
requirements that may arise.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

void updateLCD(void)
{
  if(setDebug & lcdDebug)
  {
    Serial.println("Writing to LCD");
  }
  lcd.home();
  IPAddress ip = WiFi.localIP();
  sprintf(lcdBuffer, "%d.%d.%d.%d:%d", ip[0], ip[1], ip[2], ip[3], udpPort);
  lcd.print(lcdBuffer);
  delay(delayVal);
  lcd.setCursor(0, 1);
  switch(mode)
  {
    case 'A':
    {
      lcd.print("Automatic           ");      
      break;
    }
    
    case 'M':
    {
      lcd.print("Manual              ");      
      break;
    }
    
    default:
    {
      lcd.print("Uninitialized       ");      
      break;
    }
  }
  delay(delayVal);
  lcd.setCursor(0, 2);
  sprintf(lcdBuffer, "Temp: %d F, %d C", ds18b20.tempFahrenheit, ds18b20.tempCelsius);
  lcd.print(lcdBuffer);
  delay(delayVal);
  if(setDebug & lcdDebug)
    Serial.println(lcdBuffer);
  lcd.setCursor(0, 3);
  sprintf(lcdBuffer, "Switch1:%c Switch2:%c", ds2406[0].switchStatus, ds2406[1].switchStatus);
  lcd.print(lcdBuffer);
  delay(delayVal);
  if(setDebug & lcdDebug)
  {
    Serial.println(lcdBuffer);
    Serial.println("Finished writing to LCD");
  }
}
