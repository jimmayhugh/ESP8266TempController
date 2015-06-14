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

void getTemp(uint8_t x)
{
  if(setDebug > 0)
  {
    Serial.println("Getting Temp");
  }
  ds.reset();
  ds.select(chip[x]);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  ds18Val = x;
  ds18.attach_ms(cDelayVal, readTemp);
}

void readTemp(void)
{ 
//  delay(cDelayVal);     // 9-bit resolution, maybe 250ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  ds.reset();
  ds.select(chip[ds18Val]);
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++)            // we need 9 int8_ts
  {
    data[i] = ds.read();
  }

  if(setDebug > 0)
  {
    Serial.print("  Data = ");  
    for ( i = 0; i < 9; i++)            // we need 9 int8_ts
    {
      if(data[i] < 0x0f)
      {
        Serial.print("0x0");
      }else{
        Serial.print("0x");
      }
      Serial.print(data[i], HEX);
      Serial.print(", ");
    }
    Serial.print("CRC=");
    Serial.print(ds.crc8(data, 8), HEX);
    Serial.println();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  
  int8_t cfg = (data[4] & 0x60);
  
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time
  
  celsius = raw >> 4;
  fahrenheit = ((celsius * 9)/5) + 32;
  
  if(setDebug > 0)
  {
    Serial.print("  Temperature = ");
    Serial.print(celsius);
    Serial.print(" Celsius, ");
    Serial.print(fahrenheit);
    Serial.println(" Fahrenheit");
  }

  ds18.detach();
}

