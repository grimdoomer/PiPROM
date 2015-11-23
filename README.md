# PiPROM
PiPROM allows you to easily read, write, and erase original Xbox EEPROM chips using a Raspberry Pi!

### How It Works
PiPROM allows your Raspberry Pi to interface with I2C based xbox EEPROM chips. You can either connect your Raspberry Pi directly to your xbox motherboard, or connect it to an I2C based EEPROM chip that has been removed from an xbox motherboard or other device. Once your pi is connected, PiPROM can easily read, write, or erase the contents of the chip. It's great for recovering lost hard drive keys for your xbox, or fixing broken motherboards. PiPROM is also compatible with all versions of the Raspberry Pi!

### Getting Started
1. The first step in getting PiPROM running is to enable the I2C interface on your Raspberry Pi. Adafruit has a very nice [tutorial](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c) on how to do this.
2. Once you have the I2C interface up and running on your Raspberry Pi it's time to get it connected to your xbox console. For xbox versions 1.0 - 1.5 it will require 3 wires to be soldered to the LPC port on your xbox motherboard, for version 1.6 it will require a few more connections that are a bit tricky. 

 You will need to connect the two I2C pins on your Raspberry Pi to the I2C pins on your xbox/I2C chip, as well as a ground wire between the two. The ground wire is very important! It acts as a reference line for the Pi to tell when a GPIO is high or low. If your Pi doesn't have the same ground reference as your xbox your results may be sporatic. The wiring chart below outlines the connections you need to make to connect your Pi to an xbox console. Note that the pins are the same on all versions of the Raspberry Pi (V1, B+, V2).

 Pin | Pi | Xbox v1.0 - 1.5
--- | --- | --- 
SDA | 3 | 14
SCL | 5 | 13
GND | 6 | 2

 If you are connecting the Raspberry Pi to an I2C EEPROM chip that is not connected to an xbox motherboard you will need to look up the datasheet for the chip to figure out the pinout. You may also need to power the chip using the Pi's 3.3 or 5v lines.

3. To test connectivity between the Pi and the EEPROM chip you can use `i2cdetect` to scan all I2C slave addresses and see what comes up. If you haven't already, install the i2c tools using the following commands:
 ```
 sudo apt-get install python-smbus
 sudo apt-get install i2c-tools
 ```
 Once the tools are installed you will need to power up your xbox console or EEPROM chip, yes, the console needs to physically be turned on. Don't worry, as long as you're not doing anything on your console at the same time that PiPROM is reading/writing to the xbox EEPROM it won't harm anything. If your xbox console frags at boot, or throws a system error, you will only have ~3 minutes to work before it will auto power off. This will be plenty of time for PiPROM to read/write the EEPROM chip.
 From a command line on the Pi run `i2cdetect -y 1` and you should see something similar to the following (output taken from my Pi connected to an xbox console):
 ```
pi@xb-chip ~/Projects/PiPROM $ sudo i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: 50 -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
```
