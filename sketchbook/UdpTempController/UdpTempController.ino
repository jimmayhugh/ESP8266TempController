/*
UdpTempController.ino

Version 0.0.2
Last Modified 06/09/2015
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

Configuration :
  Enter the ssid and password of your Wifi AP.
  Enter the port number your server is listening on.

*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <Wire.h>
#include <ESP8266MCP23017.h>
#include <ESP8266LCD.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int status = WL_IDLE_STATUS;
const char* ssid = "SSID";         // your network SSID (name)
const char* pass = "PASSWD";       // your network password

uint16_t localPort = 2652;      // local port to listen for UDP packets

char packetBuffer[512]; //buffer to hold incoming and outgoing packets
char lcdBuffer[21];

int16_t noBytes, fahrenheit, celsius, packetCnt, delayVal = 100, sDelayVal = 1000;
uint8_t const lcdChars = 20;
uint8_t const lcdRows  = 4;
int8_t i;
int8_t present = 0;
uint8_t data[15];
uint8_t chip[4][8];
uint8_t chipCnt = 0;
uint8_t chipStatus[3];

// LCD Stuff

// The shield uses the I2C SCL pin 5 and SDA pin 4 on the ESP8266.
// Make sure that SDA and SCA use 4.7K pullup resistors 
// You can connect other I2C sensors to the I2C bus and share
// the I2C bus.

ESP8266LCD lcd = ESP8266LCD(7);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

// OneWire Stuff
// OneWire Setup;
// Family codes
const uint8_t t3tcID         = 0xAA; // Teensy 3.0 1-wire slave with MAX31855 K-type Thermocouple chip
const uint8_t dsLCD          = 0x47; // Teensy 3.x 1-wire slave 4x20 HD44780 LCD
const uint8_t dsGLCDP        = 0x45; // Teensy 3.1 1-wire slave 800x400 7" GLCD with Paging
const uint8_t dsGLCD         = 0x44; // Teensy 3.1 1-wire slave 800x400 7" GLCD
const uint8_t max31850ID     = 0x3B; // MAX31850 K-type Thermocouple chip
const uint8_t ds2762ID       = 0x30; // Maxim 2762 digital k-type thermocouple
const uint8_t ds18b20ID      = 0x28; // Maxim DS18B20 digital Thermometer device
const uint8_t ds2406ID       = 0x12; // Maxim DS2406+ digital switch

// DS2406+ Digital Switch Family Code and registers
const uint8_t dsPIO_A        = 0x20;
const uint8_t dsPIO_B        = 0x40;
const uint8_t ds2406MemWr    = 0x55;
const uint8_t ds2406MemRd    = 0xaa;
const uint8_t ds2406AddLow   = 0x07;
const uint8_t ds2406AddHi    = 0x00;
const uint8_t ds2406PIOAoff  = 0x3f;
const uint8_t ds2406PIOAon   = 0x1f;
const uint8_t ds2406End      = 0xff;

const uint8_t switchStatusON   = 'N';
const uint8_t switchStatusOFF  = 'F';

const uint8_t oneWireAddress = 2; // OneWire Bus Address - use pin 2 for TeensyNet board
const uint8_t chipAddrSize   = 8; // 64bit OneWire Address
const uint8_t chipNameSize   = 15;

OneWire  ds(oneWireAddress);  // on pin 2 (a 4.7K resistor is necessary)

// mDNS stuff
// multicast DNS responder
MDNSResponder mdns;


void setup(void)
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(sDelayVal);
  
  lcd.begin(lcdChars, lcdRows);
  lcd.clear();
  lcd.home();

  // setting up Station AP
  WiFi.begin(ssid, pass);
  
  // Wait for connect to AP
  Serial.print("[Connecting]");
  Serial.print(ssid);
  int tries=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries > 30){
      break;
    }
  }
  Serial.println();

  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!mdns.begin("esp8266", WiFi.localIP()))
  {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");


  printWifiStatus();

  Serial.println("Connected to wifi");
  Serial.print("Udp server started at port ");
  Serial.println(localPort);
  Udp.begin(localPort);
  
}

void loop(void)
{
  noBytes = Udp.parsePacket();

  scanChips();
  
  for(uint8_t b = 0; b < 3; b++)
  {
    Serial.print("chip[");
    Serial.print(b);
    Serial.print("] =");
    for( i = 0; i < 8; i++)
    {
      Serial.write(' ');
      Serial.print(chip[b][i], HEX);
    }
    Serial.println();
  }
  
  updateLCD();

  if ( noBytes ) 
  {
    processUDP();
  } // end if

}


