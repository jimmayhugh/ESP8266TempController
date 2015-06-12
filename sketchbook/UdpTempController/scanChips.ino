/*
UdpTempController - scanChips.ino

Version 0.0.3
Last Modified 06/12/2015
By Jim Mayhugh

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

void scanChips(void)
{
  chipCnt = 0;
  do{
    // the first ROM byte indicates which chip
    switch (chip[chipCnt][0])
    {
      case ds18b20ID:
        if(setDebug > 0)
        {
          Serial.println("  Chip = DS18B20");
        }
        getTemp(chipCnt);
        break;

      case ds2406ID:
        if(setDebug > 0)
        {
          Serial.println("  Chip = DS2406+");
        }
        getState(chipCnt);
        break;

      default:
        Serial.println("Device is not a valid family device.");
        return;
     }
     chipCnt++;
  }while(chipCnt < 3);
}

