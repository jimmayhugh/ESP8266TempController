/*
UdpTempController - getTemp.ino

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

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

void getTemp(void)
{
  if(tempConversion == FALSE)
  {
    if(setDebug & tempDebug)
      Serial.println("tempConversion is FALSE, Starting Temp Conversion");
    ds.reset();
    ds.select(ds18b20.tempAddr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end
    tempConversion = TRUE;
    tempDelay = millis();
    ds18.attach_ms(cDelayVal, tempReady);
  }else{
    if(setDebug & tempDebug)
      Serial.println("tempConversion is TRUE");
  }
}

void tempReady(void)
{
  ds18.detach();
  readTemp();
}

void readTemp(void)
{ 
  if(setDebug & tempDebug)
  {
    Serial.print("tempDelay = ");
    Serial.println(millis() - tempDelay);
    Serial.println("Reading Temp");
  }
  ds.reset();
  ds.select(ds18b20.tempAddr);
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < tempDataSize; i++)            // we need 9 int8_ts
  {
    ds18b20.tempData[i] = ds.read();
  }

  if(setDebug & tempDebug)
  {
    Serial.print("  Data = ");  
    for ( i = 0; i < tempDataSize; i++)            // we need 9 int8_ts
    {
      if(ds18b20.tempData[i] < 0x0f)
      {
        Serial.print("0x0");
      }else{
        Serial.print("0x");
      }
      Serial.print(ds18b20.tempData[i], HEX);
      Serial.print(", ");
    }
    Serial.print("CRC=");
    Serial.print(ds.crc8(ds18b20.tempData, 8), HEX);
    Serial.println();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (ds18b20.tempData[1] << 8) | ds18b20.tempData[0];
  
  int8_t cfg = (ds18b20.tempData[4] & 0x60);
  
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time
  
  ds18b20.tempCelsius = raw >> 4;
  ds18b20.tempFahrenheit = (((raw >> 4) * 9)/5) + 32;
  
  if(setDebug & tempDebug)
  {
    Serial.print("  Temperature = ");
    Serial.print(ds18b20.tempCelsius);
    Serial.print(" Celsius, ");
    Serial.print(ds18b20.tempFahrenheit);
    Serial.println(" Fahrenheit");
  }
  tempConversion = FALSE;

}

