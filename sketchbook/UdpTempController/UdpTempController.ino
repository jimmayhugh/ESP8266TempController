/*
 * 29 May 2015
 *
 * Configuration : Enter the ssid and password of your Wifi AP. Enter the port number your server is listening on.
 *
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
const char* ssid = "GMJLinksys";  //  your network SSID (name)
const char* pass = "ckr7518t";       // your network password
// const char* response = "OK\n";

uint16_t localPort = 2652;      // local port to listen for UDP packets

char packetBuffer[512]; //buffer to hold incoming and outgoing packets
char lcdBuffer[21];

int16_t fahrenheit, celsius, packetCnt, delayVal = 100, sDelayVal = 1000;
uint8_t const lcdChars = 20;
uint8_t const lcdRows  = 4;
int8_t i;
int8_t present = 0;
uint8_t data[15];
uint8_t addr[8];
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
  int noBytes = Udp.parsePacket();

  scanChips();
  
  updateLCD();

  if ( noBytes ) 
  {
    Serial.print(millis() / 1000);
    Serial.print(":Packet of ");
    Serial.print(noBytes);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.print(":");
    Serial.println(Udp.remotePort());
    
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,noBytes); // read the packet into the buffer

    // display the packet contents in HEX
    for (int i=1;i<=noBytes;i++)
    {
      Serial.print(packetBuffer[i-1],HEX);
      if (i % 32 == 0)
      {
        Serial.println();
      }else Serial.print(' ');
    } // end for
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    packetCnt = sprintf(packetBuffer, "%d, %d, %c, %c\n", celsius, fahrenheit, chipStatus[1], chipStatus[2]);
    Udp.write(packetBuffer, packetCnt);
    Udp.endPacket();
  } // end if

}


