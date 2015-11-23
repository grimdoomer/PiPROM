# PiPROM
PiPROM allows you to easily read, write, and erase original Xbox EEPROM chips using a Raspberry Pi!

### How It Works
PiPROM allows your Raspberry Pi to interface with I2C based xbox EEPROM chips. You can either connect your Raspberry Pi directly to your xbox motherboard, or connect it to an I2C based EEPROM chip that has been removed from an xbox motherboard or other device. Once your pi is connected, PiPROM can easily read, write, or erase the contents of the chip. It's great for recovering lost hard drive keys for your xbox, or fixing broken motherboards. PiPROM is also compatible with all versions of the Raspberry Pi!

### Getting Started
The first step in getting PiPROM running is to enable the I2C interface on your Raspberry Pi. This can be done by running '''sudo raspi-config''' from the command line of you pi.
