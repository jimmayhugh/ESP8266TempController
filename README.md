#UDPTempController
An ESP8266-based WiFi Temperature Controller that uses Avahi for network discovery, 1-Wire DS18B20 Digital Thermometers, DS2406+ Digital Switches, and an I2C-controlled 20x4 LCD.

## Programming Environment
All of my coding was done using an ESP-12 board and the ESP8266 Arduino IDE described at <a href="https://github.com/esp8266/Arduino" target="_blank">https://github.com/esp8266/Arduino</a>

and using a simple <a href="http://smile.amazon.com/gp/product/B00F2F5HVK?psc=1&redirect=true&ref_=oh_aui_detailpage_o02_s00" target ="_blank">Black PL2303HX USB To TTL To UART RS232 COM Cable Module Converter</a> to upload the program.

## Setup
Before using the code for the first time, I recommend clearing the EEPROM on the ESP8266 to insure that it can be set up properly. I've included 'eeprom_clear.ino' in the utilities folder for that reason.

Attach your DS18B20 Digital Thermometer and DS2406+ switches, and I2C 20x4 LCD.
 
Once the EEPROM has been cleared, compile and upload the 'UdpTempController.ino' files. Once uploaded, make surethat the Serial monitor is open and set to '112500 baud', and 'Newline' for any text sent.

The Serial Monitor will ask for an SSID, PASSWD and UDP port. Set these values to match your WiFi Network.

If all goes well, the Serial Monitor should show the results of the attempt to access your WiFi Network.

## Using the UDPTempController

I've also included a short PHP utility that will communicate with the UdpTempController and allow the state of the chips to be monitored and set.

To use it, you'll need PHP5 and avahi-resolve installed.

On the command line enter" php TestUdpTempController.php esp8266.local

The program will attempt to locate the UdpTempController, and if successful, will display the IP address of the UdpTempController, and prompt for input.
