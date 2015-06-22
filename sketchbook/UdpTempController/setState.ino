/*
UdpTempController - setState.ino

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

void setState(uint8_t x, uint8_t state)
{
  if(setDebug & switchDebug)
  {
  Serial.print("Setting ");
    switch(x)
    {
      case upper:
      {
        Serial.print("upper LED ");
        break;
      }
      
      case lower:
      {
        Serial.print("lower LED ");
        break;
      }
      
      default:
      {
        Serial.print(" UNKNOWN LED ");
        Serial.print(x);
        break;
      }
    }
    switch(state)
    {
      case ds2406PIOAon:
      {
        Serial.println("ON");
        break;
      }

      case ds2406PIOAoff:
      {
        Serial.println("OFF");
        break;
      }

      default:
      {
        Serial.println("UNKNOWN");
        break;
      }
    }
    Serial.print("Switch[");
    if(x == lower)
    {
      Serial.print("lower] = ");
    }else{
      Serial.print("upper] = ");
    }
    for(int q = 0; q < chipAddrSize; q++)
    {
      if(ds2406[x].switchAddr[q] < 0x10)
      {
        Serial.print("0x0");
      }else{
        Serial.print("0x");
      }
      Serial.print(ds2406[x].switchAddr[q], HEX);
      if(q < chipAddrSize - 1)
      {
        Serial.print(", ");
      }else{
        Serial.println();
      }
    }
  }
  ds.reset();
  ds.select(ds2406[x].switchAddr);
  ds.write(ds2406MemWr);
  ds.write(ds2406AddLow);
  ds.write(ds2406AddHi);
  ds.write(state);
  for ( int i = 0; i < 6; i++)
  {
    data[i] = ds.read();
  }
  ds.write(ds2406End);
  ds.reset();
  
}
