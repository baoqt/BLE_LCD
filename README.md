# BLE_LCD
___
Using the RN4871 module, the microcontroller receives messages from a computer and displays text on the LCD.
___
### Primary goals
Establish connection with computer terminal or mobile device.

Reliably receive messages.

Stay within program memory limits, possibly offloading some code to the transmitter side.
___
### Planned additions
Removing the dedicated programming header if possible.

Switch over to serial USB programming for the PIC.

Add battery power and regulation.
___
### Relevant datasheets
BLE module:

http://ww1.microchip.com/downloads/en/DeviceDoc/RN4870-71-Bluetooth-Low-Energy-Module-Data-Sheet-DS50002489D.pdf

BLE module programming:

http://ww1.microchip.com/downloads/en/DeviceDoc/50002466B.pdf

PIC microcontroller:

http://ww1.microchip.com/downloads/en/devicedoc/pic16(l)f1831318323%20full%20featured%20low%20pin%20count%20microcontrollers%20with%20xlp_40001799d.pdf

LCD display:

http://www.newhavendisplay.com/specs/NHD-C12832A1Z-NSW-BBW-3V3.pdf

LCD display programming:

http://www.newhavendisplay.com/specs/NHD-C12832A1Z-NSW-BBW-3V3.pdf
