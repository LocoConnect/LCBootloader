avr_boot SD Bootloader for Arduino for LocoConnect
==========

The avr_boot bootloader loads a program from the SD card on startup. This documentation is specific to [Arduino](http://arduino.cc) IDE boards installation. The main (source) documentation is located [here](https://github.com/zevero/avr_boot).

## Table of Contents
- [Installation](#installation)
  - [Manual Installation](#manual-installationrequires-arduino-ide-version-161-or-greater)
- [Using the Board Menu Entries](#using-the-board-menu-entries)
  - [BOD Menu](#bod-menu)
  - [SD CS Pin Menu](#sd-cs-pin-menu)
  - [Clock Menu](#clock-menu)
- [Burning the Bootloader](#burning-the-bootloader)
- [Uploading Your Sketch](#uploading-your-sketch)
- [Troubleshooting](#troubleshooting)
- [Supported Microcontrollers](#supported-microcontrollers)
- [Supported Boards](#supported-boards)
- [Acknowledgments](#acknowledgments)

## Installation
**WARNING: avr_boot is not compatible with Arduino AVR Boards 1.6.12(included with Arduino IDE 1.6.10).**

#### Manual Installation(requires Arduino IDE version 1.6.1 or greater)
- Download the avr_boot files here: https://github.com/LocoConnect/LCBootloader
- Extract the .zip file.
- Move the **avr_boot** folder into the packages folder. For Arduino IDE 2.0 on Windows this is: **%LOCALAPPDATA%\arduino15\packages**
- If the Arduino IDE is running then restart it.

## Using the Board Menu Entries
After installing avr_boot for LocoConnect a new board is added to the **LocoConnect** section of the **Tools** > **Board** menu. When this board is selected additional menus will appear under the **Tools** menu.

Whenever you change a setting in these menus you need to **[Burn Bootloader](#burning-the-bootloader)** to reconfigure your board.

#### BOD Menu
BOD stands for Brown-out Detection. This feature is intended to avoid improper operation caused by insufficient supply voltage. If the supply voltage drops below the BOD value then the microcontroller is reset.

#### Model Menu
Displays a list of board models for your **Board** menu selection. You don't need to re-burn the bootloader after changing the model menu selection

#### SD CS Pin Menu
The SD CS pin may be connected to different Arduino pins depending on which shield you are using or your wiring preference if not using a shield.
- Ethernet Shield: **4**
- Arduino Ethernet: **4**
- Seeed SD card shield: **4**
- SparkFun MicroSD Shield: **8**
- Adafruit Data Logger Shield: **10**

#### Clock Menu
Note that ATmel has recently removed full swing support from some of the AVR models(http://img.digikey.com/Retractions/WC154601%20-%20PCN-12-23-2015.pdf).
- **16MHz External Low Power** - This is usually the best setting to use for commercially produced boards that run at 16MHz as it will decrease power usage compared to the **16MHz Full Swing** setting.
- **16MHz External Full Swing** - This setting can be used for breadboard or homemade 16MHz boards where the **16MHz Low Power** setting causes unreliability due to electrical interference.

## Burning the Bootloader
To burn the bootloader, you will need an ISP(in-system programmer). After you have connected the Arduino board and the programmer to your computer navigate to the **Tools** > **Board** menu and select the correct board. Then go to **Tools** > **Programmer** and select the programmer you are using. In case you are using **Arduino as ISP**, make sure that the selected port in the **Tools** > **Serial Port** menu refers to the **Arduino as ISP** board and not the board that you want to burn the bootloader on. Now, just launch the **Tools** > **Burn Bootloader** command and wait until the operation completes. You will no longer be able to upload sketches to your board using USB once the avr_boot bootloader is installed. To go back to normal uploading repeat the bootloader burning process with a non-avr_boot board selected.

## Uploading Your Sketch
- In Arduino IDE go to File > Preferences and check "Show verbose output during compiliation"
- compile LocoConnect sketch and find the location of your /tmp/buildxxx/lococonnect.cpp.hex
- make bin file: avr-objcopy -I ihex -O binary sketch.cpp.hex LCONNECT.BIN
- copy the file into the root of an SD (FAT32)
- put it into the SD slot and connect the SD card to the SPI interface of your LocoConnect board
- reset it
- it might already have happend!

## Troubleshooting
- `java.lang.NullPointerException`(Arduino IDE 1.6.5-r5 or previous) or `panic: runtime error: invalid memory address or nil pointer dereference`(Arduino IDE 1.6.6 or higher) error while compiling/uploading with ATmega1284/P or ATmega644/P/A/PA.
  - Your **Pinout** menu selection requires the installation of [Mighty 1284P](https://github.com/JChristensen/mighty-1284p/tree/v1.6.3) or [MightyCore](https://github.com/MCUdude/MightyCore). Either install it or choose a different pinout.
- avr_boot boards don't appear in the **Tools > Board** menu after manual installation.
  - avr_boot requires Arduino IDE 1.6.1 or higher.
- `avrdude: verification error; content mismatch` error during lock verification while burning bootloader with AVRISP mkII or other STK500v2 programmer.
  - avr_boot is not compatible with Arduino AVR Boards 1.6.12. Please use any other version of Arduino AVR Boards.
- avr_boot entry doesn't appear in Boards Manager when using Arduino IDE 1.6.6.
  - Close Boards Manager and then open it again.
- `Bootloader file specified but missing` warning when compiling when using Arduino IDE 1.6.6.
  - Please ignore, this is caused by a bug in that IDE version and will not cause any problems.
-  `Warning: This core does not support exporting sketches. Please consider upgrading it or contacting its author` when doing **Sketch > Export compiled Binary** while using Arduino IDE 1.6.5 on Windows.
  - This is caused by a bug in Arduino IDE 1.6.5. Please either use the [alternate method](https://github.com/zevero/avr_boot#put-your-sketch-on-sd-card) of obtaining the compiled .hex file or upgrade to a more recent version of the Arduino IDE.
- `avrdude: AVR Part "build.mcu" not found.` or `avr-g++: error: unrecognized argument in option '-mmcu=build.mcu'`.
  - avr_boot requires Arduino IDE 1.6.1 or greater,
- `avrdude: AVR Part "atmega644a" not found.` or similar error during **Upload** or **Upload Using Programmer** to ATmega644A, ATmega644PA, ATmega1284, ATmega32A, ATmega128A or ATmega64A.
  - This is caused by a limitation of the Arduino IDE that does not allow avr_boot's custom avrdude.conf file to be used for these actions when one of the Arduino AVR Boards programmers is selected.  Since it doesn't interfere with avr_boot's intended usage for SD card program uploads, the issue will not be fixed at this time. You can solve the issue by installing [MightyCore](https://github.com/MCUdude/MightyCore) or [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore) and selecting the version of your programmer marked `(MightyCore)` or `(ATtiny)` from the **Tools > Programmer** menu.
- Using non-A*n* analog pin numbers notation with `analogRead()` doesn't work correctly(e.g. 4 instead of A4).
  - This is a known limitation of Arduino's `analogRead()` function with non-standard microcontroller parts. Please use A*n* pin notation or use the equivalent digital pin number(e.g. 14 for A0 on Uno).
- An Arduino core or other library doesn't work correctly with one of the microcontrollers supported by avr_boot
  - avr_boot is a bootloader, not a core. The Arduino AVR Boards core and the Arduino libraries are used by avr_boot. These will often work well for non-standard microcontrollers but not always. For improved support of ATmega1284/644/324/32 microcontrollers please use [MightyCore](https://github.com/MCUdude/MightyCore). For improved support of ATmega128/64 please use [MegaCore](https://github.com/MCUdude/MegaCore). You can use the avr_boot bootloader with the files generated by compiling with these cores.
- My SD card reader's CS line is connected to to a pin not included in the menu.
  - Please compile avr_boot from source. You can set any SD CD pin you like.
- I found a bug.
  - Make sure you are using the latest version of avr_boot and the latest version of the Arduino IDE.
  - Spend some time troubleshooting and doing research to verify the issue and make sure it's caused by avr_boot.
  - Do a search of https://github.com/zevero/avr_boot/issues to make sure your issue hasn't already been reported.
  - Report the issue at https://github.com/zevero/avr_boot/issues or submit a pull request at https://github.com/zevero/avr_boot/pulls
    - Please give as much information as possible, including:
      - avr_boot version
      - Arduino IDE version
      - Operating system
      - Installation method(Boards Manager, Manual, or compile from source)
      - Which selections you are using in the **Tools > Board**, **Tools > Processor**, **Tools > Pinout**, **Tools > Clock**, **Tools > BOD** menus.
      - Complete error messages.
      - Full description of the steps required to reproduce the issue.

## Supported Microcontrollers:
- ATmega328P

## Supported Boards:
- ATmega328P based:
  - LocoConnect V1.0 and later
  - LocoConnectSMD V1.0 and later

## Acknowledgments
- [David A. Mellis](https://github.com/damellis) - avr_developers variant
- [Steve Marple](https://github.com/stevemarple) - Calunium PCB and Stripboard variants
- [Mighty 1284P](https://github.com/JChristensen/mighty-1284p) - Standard, Bobuino, and Sleeping Beauty variants
- [MCUdude](https://github.com/MCUdude) - [MegaCore](https://github.com/MCUdude/MegaCore) variant for ATmega64/A, 128/A
- [Phillip Stevens](https://github.com/feilipu) - Goldilocks v1.1 variant
- [WickedDevice](https://github.com/WickedDevice) - WildFire v2/v3 variants
- [Adafruit](https://github.com/adafruit) - adafruit32u4, bluefruitmicro, circuitplay32u4, feather32u4, flora variants
- [SparkFun](https://github.com/sparkfun) - promicro variant
- [Others listed in the main documentation](https://github.com/zevero/avr_boot#thanks-to)
