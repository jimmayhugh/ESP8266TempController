#UDPTempController
An ESP8266-based WiFi Temperature Controller that uses Avahi for network discovery, 1-Wire DS18B20 Digital Thermometers, DS2406+ Digital Switches, and an I2C-controlled 20x4 LCD.

## Programming Environment
All of my coding was done using an ESP-12 board and the ESP8266 Arduino IDE described at
 
Before using the 
I've also included a short PHP utility that will communicate with the UdpTempController and allow the state of the chips to be monitored and set.

To use it, you'll need PHP5 and avahi-resolve installed.

On the command line enter" php TestUdpTempController.php esp8266.local

The program will attempt to locate the UdpTempController, and if successful, will display the IP address of the UdpTempController, and prompt for input.
