#UDPTempController
An ESP8266-based WiFi Temperature Controller that uses Avahi for network discovery, 1-Wire DS18B20 Digital Thermometers, DS2406+ Digital Switches, and an I2C-controlled 20x4 LCD.

##Programming Environment
All of my coding was done using an ESP-12 board and the ESP8266 Arduino IDE described at <a href="https://github.com/esp8266/Arduino" target="_blank">https://github.com/esp8266/Arduino</a> and using a simple <a href="http://smile.amazon.com/gp/product/B00F2F5HVK?psc=1&redirect=true&ref_=oh_aui_detailpage_o02_s00" target ="_blank">Black PL2303HX USB To TTL To UART RS232 COM Cable Module Converter</a> to upload the program.

The code also works with the ESP-01 board, but the I2C lines are not physically available, so the LCD monitor is not used with that configuration.

##Setup
Before using the code for the first time, I recommend clearing the EEPROM on the ESP8266 to insure that it can be set up properly. I've included 'eeprom_clear.ino' in the utilities folder for that reason.

Attach your DS18B20 Digital Thermometer and DS2406+ switches, and I2C 20x4 LCD.
 
Once the EEPROM has been cleared, compile and upload the 'UdpTempController.ino' files. Once uploaded, make surethat the Serial monitor is open and set to '112500 baud', and 'Newline' for any text sent.

The Serial Monitor will ask for an SSID, PASSWD. Set these values to match your WiFi Network.

The Serial Monitor will then ask is you want to use DHCP to get an IP address. If you answer "Y", the ESP8266 will attempt to negotiate an IP address via DHCP. A "N" answer will result in a request for an IP address, Gateway address and Subnet Mask. These values MUST be enterd as "XXX.XXX.XXX.XXX" with leading zeros where the "XXX" value is less than 100, ie "002".

The Serial Monitor will then ask for a udpPort value. This is the port address that the board will use when communicating with the network.

If all goes well, the Serial Monitor should show the results of the attempt to access your WiFi Network.

##Using the UDPTempController

I've also included a short PHP utility that will communicate with the UdpTempController and allow the state of the chips to be monitored and set.

To use it, you'll need PHP5 and avahi-resolve installed.

On the command line enter:
`php TestUdpTempController.php esp8266.local`

The program will attempt to locate the UdpTempController, and if successful, will display the IP address of the UdpTempController, and prompt for input.

##Commands
In general, commands take the form of a single Uppercase Letter command, followed by a numeric or alpha value. single character values immediately proceed the command with no intervening spaces.
As an example, to turn on the first switch in maual mode (more about modes in a moment), the command would be `AN`.

Commands that include a numeric value have no intervening spaces. An example would be setting the Upper Temperature Value in Automatic mode: `UF80`, which sets the Upper Temperature Value to 80&deg;F

Commands that require a multi-character alpha-numeric string have an intervening space.
As an example, to re-name the local url of the UdpTempController, the command would be `N "DomainName"`.

##Web Page and MySql
Included are web pages and a MySql file which can be used to create a web page that can monitor multiple ESP boards, display the results on a web page, save the results into a set of MySQL tables, and display a graph of the results. The updates to the page occur every two seconds, and updated data is inserted into the database every minute.
