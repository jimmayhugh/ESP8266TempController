/*
UdpTempController - updateState.ino

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

void updateState(void)
{
  if(mode == 'A')
  {
    if(ds18b20.tempFahrenheit >= upperF)
    {
      if(ds2406[upper].switchDelay == 0)
      {
        setState(upper, ds2406PIOAon); // cool things off
        if(setDebug & switchDebug)
          Serial.println("Set upper Switch immediate- ON");
      }else if(ds2406[upper].switchDelaySet == FALSE){
        if(setDebug & switchDebug)
        {
          Serial.print("set upperDelay = ");
          Serial.println(ds2406[upper].switchDelay);
        }
        ds2406[upper].switchDelaySet = TRUE;
        ds2406_0.attach_ms((ds2406[upper].switchDelay * 1000), startUpper);
        startUpperTimer = millis();
      }
    }else{
      if(setDebug & switchDebug)
        Serial.println("Set upper - OFF, ");
      setState(upper, ds2406PIOAoff);
    }

    if(ds18b20.tempFahrenheit <= lowerF)
    {
      if(ds2406[lower].switchDelay == 0)
      {
        setState(lower, ds2406PIOAon); // heat things up
        if(setDebug & switchDebug)
          Serial.println("Set lower Switch immediate - ON");
      }else if(ds2406[lower].switchDelaySet == FALSE){
        if(setDebug & switchDebug)
        {
          Serial.print("set lowerDelay = ");
          Serial.println(ds2406[lower].switchDelay);
        }
        ds2406[lower].switchDelaySet = TRUE;
        ds2406_1.attach_ms((ds2406[lower].switchDelay * 1000), startLower);
        startLowerTimer = millis();
      }
    }else{
      if(setDebug & switchDebug)
        Serial.println("lower - OFF");
      setState(lower, ds2406PIOAoff);
    }
  }
}

void startUpper(void)
{
  ds2406_0.detach();
  if(ds18b20.tempFahrenheit >= upperF)
  {
    setState(upper, ds2406PIOAon); // cool things off
  }else{
    setState(upper, ds2406PIOAoff);
  }
  ds2406[upper].switchDelaySet = FALSE;
  if(setDebug & switchDebug)
  {
    Serial.printf("set upper switch ON after a ");
    Serial.print(millis() - startUpperTimer);
    Serial.println(" delay");
  }
}

void startLower(void)
{
  ds2406_1.detach();
  if(ds18b20.tempFahrenheit <= lowerF)
  {
    setState(lower, ds2406PIOAon); // heat things up
  }else{
    setState(lower, ds2406PIOAoff);
  }
  ds2406[lower].switchDelaySet = FALSE;
  if(setDebug & switchDebug)
  {
    Serial.printf("set lower switch ON after a ");
    Serial.print(millis() - startLowerTimer);
    Serial.println(" delay");
  }
}
