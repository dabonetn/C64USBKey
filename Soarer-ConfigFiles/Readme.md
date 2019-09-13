Here's a config file for the C64USBKey hardware (Pro micro only guys!)


Works just like the original C64USBKey, except the second hybrid key is runstop (alt), not CTRL (Tab)

This is a firmware limitation of Soarer, and it only allows modifier keys to activate another layer.




So for install, first you have to flash your pro micro with soarer's firmware.

Quick and dirty instructions are available here.


https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/blob/master/C64_keyboard/README.md#pins-i-used


Here's the command I used to flash. Don't forget to use the correct serial port.

avrdude.exe -C ..\etc\avrdude.conf -v -p m32u4 -c avr109 -P %1 -U flash:w:Soarer_Controller_v1.20_beta4_atmega32u4.hex:i



And when you get to the setup portion, sub the C64USBkeyconfig.sc for his config file.


Then also remember to copy the rpi_sym.vkm into the c64 dir on the bmc64 microsd.

Currently runstop doesn't work, BMC64 should be fixed soon so the alt key is seen.

For joysticks, just go to the menu (Either commodore-f7 or shift restore) and config the custom keysets, and map the joysticks to the keysets.
















