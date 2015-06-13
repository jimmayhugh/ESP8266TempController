/*
UdpTempController - showEEPROM.ino

Version 0.0.1
Last Modified 06/11/2015
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

void showEEPROM(void)
{
  EEPROM_readAnything(EELowerC, lowerC);
  EEPROM_readAnything(EELowerF, lowerF);
  EEPROM_readAnything(EEUpperC, upperC);
  EEPROM_readAnything(EEUpperF, upperF);
  EEPROM_readAnything(EEMode, mode);
  
  Serial.print("lowerC = ");
  Serial.print(lowerC);
  Serial.print(", lowerF = ");
  Serial.print(lowerF);
  Serial.print(", upperC = ");
  Serial.print(upperC);
  Serial.print(", upperF = ");
  Serial.print(upperF);
  Serial.print(", mode = ");
  switch(mode)
  {
    case 'A':
    {
      Serial.println("Automatic");      
      break;
    }
    
    case 'M':
    {
      Serial.println("Manual");      
      break;
    }
    
    default:
    {
      Serial.println("Uninitialized");      
      break;
    }
  }
}

void updateEEPROM(uint16_t level)
{
  switch(level)
  {
    case EELowerC:
    case EELowerF:
    {
      EEPROM_writeAnything(EELowerC, lowerC);
      EEPROM_writeAnything(EELowerF, lowerF);
      break;
    }
    
    case EEUpperC:
    case EEUpperF:
    {
      EEPROM_writeAnything(EEUpperC, upperC);
      EEPROM_writeAnything(EEUpperF, upperF);
      break;
    }
    
    case EEMode:
    {
      EEPROM_writeAnything(EEMode, mode);
      break;
    }
  }
  EEPROM.commit();
  showEEPROM();
}
