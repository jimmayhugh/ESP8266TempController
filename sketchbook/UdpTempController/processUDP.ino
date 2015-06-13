/*
UdpTempController - processUDP.ino

Version 0.0.3
Last Modified 06/11/2015
By Jim Mayhugh

V0.0.3 - Added Upper (U) and Lower (L) Temp values

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

void processUDP(void)
{
  if(setDebug > 0)
  {
    Serial.print(millis() / 1000);
    Serial.print(":Packet of ");
    Serial.print(noBytes);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.print(":");
    Serial.println(Udp.remotePort());
  }
  
  // We've received a packet, read the data from it
  Udp.read(packetBuffer,noBytes); // read the packet into the buffer

  // display the packet contents in HEX
  if(setDebug > 0)
  {
    for (int i=1;i<=noBytes;i++)
    {
      Serial.print(packetBuffer[i-1],HEX);
      if (i % 32 == 0)
      {
        Serial.println();
      }else Serial.print(' ');
    } // end for
  }

  switch(packetBuffer[0])
  {
    case 'A':
    {
      if(packetBuffer[1] == 'N')
      {
        setState(1, ds2406PIOAon);
        packetCnt = sprintf(packetBuffer, "%s", "Turning LED A ON");
      }else{
        setState(1, ds2406PIOAoff);
        packetCnt = sprintf(packetBuffer, "%s", "Turning LED A OFF");
      }
      break;
    }

    case 'B':
    {
      if(packetBuffer[1] == 'N')
      {
        setState(2, ds2406PIOAon);
        packetCnt = sprintf(packetBuffer, "%s", "Turning LED B ON");
      }else{
        setState(2, ds2406PIOAoff);
        packetCnt = sprintf(packetBuffer, "%s", "Turning LED B OFF");
      }
      break;
    }

    case 'L':
    {
      if( (isdigit(packetBuffer[2]) ) ||
          ((packetBuffer[2] == '-') && isdigit(packetBuffer[3]))
        )
      { 
        switch(packetBuffer[1])
        {
          case 'C':
          {
            lowerC = atoi((char *) &packetBuffer[2]);
            lowerF = (((lowerC * 9) / 5) + 32);
            updateEEPROM(EELowerC);
            packetCnt = sprintf(packetBuffer, "Lower value set to %d C, %d F", lowerC, lowerF);
            break;
          }

          case 'F':
          {
            lowerF = atoi((char *) &packetBuffer[2]);
            lowerC = (((lowerF - 32) * 5) / 9); 
            updateEEPROM(EELowerF);
            packetCnt = sprintf(packetBuffer, "Lower value set to %d F, %d C", lowerF, lowerC);
            break;
          }
          
          default:
          {
            packetCnt = sprintf(packetBuffer, "Invalid Option - Lower value is %d F, %d C", lowerF, lowerC);
            break;
          }
        }
      }else{
        packetCnt = sprintf(packetBuffer, "Invalid Option - Lower value is %d F, %d C", lowerF, lowerC);
      }
      break;
    }

    case 'M':
    {
      switch(packetBuffer[1])
      {
        case 'A':
        {
          mode = 'A';
          packetCnt = sprintf(packetBuffer, "Mode is %c - Automatic", mode);
          break;
        }

        case 'M':
        {
          mode = 'M';
          packetCnt = sprintf(packetBuffer, "Mode is %c - Manual", mode);
          break;
        }

        default:
        {
          packetCnt = sprintf(packetBuffer, "Invalid Option - mode is %c", mode);
          break;
        }
      }
      updateEEPROM(EEMode);
      break;
    }

    case 'U':
    {
      if( (isdigit(packetBuffer[2]) ) ||
          ((packetBuffer[2] == '-') && isdigit(packetBuffer[3]))
        )
      { 
        switch(packetBuffer[1])
        {
          case 'C':
          {
            upperC = atoi((char *) &packetBuffer[2]);
            upperF = (((upperC * 9) / 5) + 32); 
            updateEEPROM(EEUpperC);
            packetCnt = sprintf(packetBuffer, "Upper value set to %d C, %d F", upperC, upperF);
            break;
          }

          case 'F':
          {
            upperF = atoi((char *) &packetBuffer[2]);
            upperC = (((upperF - 32) * 5) / 9); 
            updateEEPROM(EEUpperF);
            packetCnt = sprintf(packetBuffer, "Upper value set to %d F, %d C", upperF, upperC);
            break;
          }
          
          default:
          {
            packetCnt = sprintf(packetBuffer, "Invalid Option - Upper value is %d F, %d C", upperF, upperC);
            break;
          }
        }
      }else{
        packetCnt = sprintf(packetBuffer, "Invalid Option - Upper value is %d F, %d C", upperF, upperC);
      }
      break;
    }

    default:
    {
      packetCnt = sprintf(packetBuffer, "%d, %d, %c, %c\n", celsius, fahrenheit, chipStatus[1], chipStatus[2]);
      break;
    }
  }
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(packetBuffer, packetCnt);
  Udp.endPacket();    
}
