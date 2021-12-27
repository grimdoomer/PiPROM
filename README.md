# PiPROM
PiPROM allows you to easily read, write, and erase original Xbox EEPROM chips using a Raspberry Pi!

### How It Works
PiPROM allows your Raspberry Pi to interface with I2C based xbox EEPROM chips. You can either connect your Raspberry Pi directly to your xbox motherboard, or connect it to an I2C based EEPROM chip that has been removed from an xbox motherboard or other device. Once your pi is connected, PiPROM can easily read, write, or erase the contents of the chip. It's great for recovering lost hard drive keys for your xbox, or fixing broken motherboards. PiPROM is also compatible with all versions of the Raspberry Pi!

### Getting Started
1. The first step in getting PiPROM running is to enable the I2C interface on your Raspberry Pi. Adafruit has a very nice [tutorial](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c) on how to do this.
2. Once you have the I2C interface up and running on your Raspberry Pi it's time to get it connected to your xbox console. For xbox versions 1.0 - 1.5 it will require 3 wires to be soldered to the LPC port on your xbox motherboard, for version 1.6 it will require a few more connections that are a bit tricky. 

 You will need to connect the two I2C pins on your Raspberry Pi to the I2C pins on your xbox/I2C chip, as well as a ground wire between the two. The ground wire is very important! It acts as a reference line for the Pi to tell when a GPIO is high or low. If your Pi doesn't have the same ground reference as your xbox your results may be sporatic. The wiring chart below outlines the connections you need to make to connect your Pi to an xbox console. The numbers refer to the RAW pin numbers on the Pi, and the LPC pins on an xbox motherboard. Note that the pins are the same on all versions of the Raspberry Pi (V1, B+, V2).

 Pin | Pi | Xbox v1.0 - 1.6
--- | --- | --- 
SDA | 3 | 14
SCL | 5 | 13
GND | 6 | 2

 ![alt text](/images/lpc_pinouts.png?raw=true)

 If you are connecting the Raspberry Pi to an I2C EEPROM chip that is not connected to an xbox motherboard you will need to look up the datasheet for the chip to figure out the pinout. You may also need to power the chip using the Pi's 3.3 or 5v lines.

3. To test connectivity between the Pi and the EEPROM chip you can use `i2cdetect` to scan all I2C slave addresses and see what comes up. If you haven't already, install the i2c tools using the following commands:
 ```
 sudo apt-get install python-smbus
 sudo apt-get install i2c-tools
 ```
 Once the tools are installed you will need to power up your xbox console or EEPROM chip, yes, the console needs to physically be turned on. Don't worry, as long as you're not doing anything on your console at the same time that PiPROM is reading/writing to the xbox EEPROM it won't harm anything. If your xbox console frags at boot, or throws a system error, you will only have ~3 minutes to work before it will auto power off. This will be plenty of time for PiPROM to read/write the EEPROM chip. Unless you are using a 1.6 version motherboard then the timing is extremely reduced.
 
 From a command line on the Pi run `sudo i2cdetect -y 1` and you should see something similar to the following (output taken from my Pi connected to an xbox console):
 
 Note: on a 1.6 console alot of this will not be available however enough to mess with the eeprom will be you will also only be able to mess with the console with a timing window of "you press power button and enter at basically the same time" give or take
 
 ![alt text](/images/i2c_xbox.png?raw=true)


 The EEPROM on the xbox console is located at address 0x54, but PiPROM already knows this! If your Pi is connected to your xbox console and you don't see output from i2cdetect that looks like the image above then something is wrong. Check your wires, and make sure you have a ground wire between your Pi and xbox console.
 
 If your Pi is connected directly to an I2C EEPROM chip you should only see one address listed in the output from i2cdetect. This is the I2C address you will later plug into PiPROM to override the default I2C address of the xbox EEPROM.

4. Next it's time to get PiPROM on to your Raspberry Pi. We will start by creating a directory for PiPROM and its dependencies. From a command line run the following commands which will create a folder called PiPROM and move you into it.
 ```
 mkdir PiPROM
 cd ./PiPROM
 ```

 Next we will setup bcm2835, a C library for the broadcom processor the Raspberry Pi uses. It is what PiPROM is built on and you will need to it compile PiPROM. If you have a Raspberry Pi 4 please use the second set of instructions instead. Run the following commands to download and install bcm2835 v1.42. If you run into trouble please refer to the [bcm2835 website](http://www.airspayce.com/mikem/bcm2835/index.html).
 ```
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.42.tar.gz
tar zxvf bcm2835-1.42.tar.gz
cd bcm2835-1.42
./configure
make
sudo make check
sudo make install
```

For Raspberry Pi 4
```
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.62.tar.gz
tar zxvf bcm2835-1.62.tar.gz
cd bcm2835-1.62
make
sudo make check
sudo make install
```

 Next we are going to download the latest source for PiPROM and compile it. Run the following commands which will move you back into the PiPROM folder and download the latest source code for it.

I added my own line into this. Use mine until the original OP fixes/merges my changes.

  ```
 cd ..
 git clone https://github.com/risk510/PiPROM.git
 cd ./PiPROM
 ```
 
 (Dont follow this unless OP merges my changes)
 ```
 cd ..
 git clone https://github.com/grimdoomer/PiPROM.git
 cd ./PiPROM
 ```
 
 Finally we will compile PiPROM for your version of Raspberry Pi. From a command line run one of the following commands to compile PiPROM for your version of the Raspberry Pi.
 
 Pi Version | Command
 --- | ---
 v1 | ```make p1```
 v1 B+ | ```make p1b```
 v2 B | ```make p2```
 v3 B | ```make p3```
 v4 | ```make p3```
 
 If everything went smoothly you should see no compiler errors in the output. To check run the ```ls``` command, and there should be a file called "PiPROM.a" in the current folder. If PiPROM.a exists then you successfully compiled PiPROM, if it doesn't then I guess I messed up somewhere... You can send me an email and I can try to see where things went wrong for you.
 
5. Now you are ready to starting reading and writing EEPROMs! Below you can find the syntax for PiPROM. Remeber your PiPROM file is currently called PiPROM.a! You will need to run PiPROM using ```sudo``` in order for it to be able to access the I2C interface.

 If your Pi is connected to an xbox console and you want to read, write, or erase the EEPROM, you can use one of the following commands:
 ```
 Read xbox eeprom to eeprom.bin:
 sudo PiPROM -r ./eeprom.bin
 
 Write eeprom.bin to the xbox:
 sudo PiPROM -w ./eeprom.bin
 
 Erase the eeprom:
 sudo PiPROM -e
 ```
 
 If your Pi is connected directly to an EEPROM not attached to an xbox motherboard you will need the I2C address you found using i2cdetect earlier.
 ```
 Read eeprom at I2C address 0x50 to eeprom.bin:
 sudo PiPROM -r -a 0x50 ./eeprom.bin
 
 Write eeprom.bin to eeprom at I2C address 0x48:
 sudo PiPROM -w -a 0x48 ./eeprom.bin
 
 Erase eeprom at I2C address 0x54:
 sudo PiPROM -e -a 0x54
 ```
 Note: You may need to replace "PiPROM" with "./PiPROM.a" on the command line if you get an error saying "PiPROM command not found". 
 
 PiPROM command line syntax:
 ```
 PiPROM v1.0 by Grimdoomer

Usage: PiPROM -r/w/e <options> <filename>
        -r              Read the EEPROM chip contents to <filename>.
        -w              Write the contents of <filename> to the EEPROM chip.
        -e              Erases the EEPROM filling it with 00s.
        filename        EEPROM file to read from/write to.

Optional:
        -a <address>    I2C address of the EEPROM chip.
 ```
 
 If you receive any errors during the reading or writing process check your wiring. Make sure your SDA, SCL, and GND wires have a strong connection between the Pi and the Xbox/EEPROM chip.

### Troubleshooting
<p>
Q: I'm having trouble getting the I2C interface setup on my Pi, what should I do?
<br>
A: Check out Adafruit's [tutorial](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c) on setting up the I2C interface.
<br><br>

Q: I'm having trouble setting up bcm2835 or version 1.42 is no longer available?
<br>
A: Check out the [bcm2835 website](http://www.airspayce.com/mikem/bcm2835/index.html) for help getting it setup, or finding a newer version to compile with.
<br><br>

Q: When I run i2cdetect no devices are detected?
<br>
A: Check and make sure you don't have your SDA/SCL wires mixed up, and make sure your ground connection between your Pi and xbox/EEPROM is good. If you are trying to use PiPROM with an xbox make sure the console is power on before runnng i2cdetect or PiPROM.
<br><br>

Q: I get errors when trying to read/write an EEPROM chip?
<br>
A: Check and make sure you don't have your SDA/SCL wires mixed up, and make sure your ground connection between your Pi and xbox/EEPROM is good.
</p>

### Changelog
* v1.0 - Initial release
  * Cleaned up source code for initial release.
  * Added the -a switch to specify an I2C address to use.

### Greets
A big thank you to Adafruit for all of their work and support for the Raspberry Pi community, and Mike McCauley for making the bcm2835 library.

For more of my projects you can visit my website: www.icode4.coffee
