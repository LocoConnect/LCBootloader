#!/bin/bash
rm *.hex

#make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=8000000 SD_CS_PORT=PORTD SD_CS_DDR=DDRD SD_CS_BIT=4 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328p_cs4_8000000L 
#make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=8000000 SD_CS_PORT=PORTB SD_CS_DDR=DDRB SD_CS_BIT=0 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328p_cs8_8000000L 
#make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=8000000 SD_CS_PORT=PORTB SD_CS_DDR=DDRB SD_CS_BIT=2 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328p_cs10_8000000L 
#make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=8000000 SD_CS_PORT=PORTC SD_CS_DDR=DDRC SD_CS_BIT=2 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328p_cs16_8000000L 

make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=16000000 SD_CS_PORT=PORTD SD_CS_DDR=DDRD SD_CS_BIT=4 BUILD_VLXVERSION=0 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328pL_cs4_16000000L 
make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=16000000 SD_CS_PORT=PORTB SD_CS_DDR=DDRB SD_CS_BIT=0 BUILD_VLXVERSION=0 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328pL_cs8_16000000L 
make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=16000000 SD_CS_PORT=PORTB SD_CS_DDR=DDRB SD_CS_BIT=2 BUILD_VLXVERSION=0 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328pL_cs10_16000000L 
make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=16000000 SD_CS_PORT=PORTC SD_CS_DDR=DDRC SD_CS_BIT=2 BUILD_VLXVERSION=0 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328pL_cs16_16000000L 

make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=16000000 SD_CS_PORT=PORTD SD_CS_DDR=DDRD SD_CS_BIT=4 BUILD_VLXVERSION=1 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328pV_cs4_16000000L 
make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=16000000 SD_CS_PORT=PORTB SD_CS_DDR=DDRB SD_CS_BIT=0 BUILD_VLXVERSION=1 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328pV_cs8_16000000L 
make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=16000000 SD_CS_PORT=PORTB SD_CS_DDR=DDRB SD_CS_BIT=2 BUILD_VLXVERSION=1 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328pV_cs10_16000000L 
make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=16000000 SD_CS_PORT=PORTC SD_CS_DDR=DDRC SD_CS_BIT=2 BUILD_VLXVERSION=1 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328pV_cs16_16000000L 


#make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=20000000 SD_CS_PORT=PORTD SD_CS_DDR=DDRD SD_CS_BIT=4 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328p_cs4_20000000L 
#make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=20000000 SD_CS_PORT=PORTB SD_CS_DDR=DDRB SD_CS_BIT=0 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328p_cs8_20000000L 
#make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=20000000 SD_CS_PORT=PORTB SD_CS_DDR=DDRB SD_CS_BIT=2 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328p_cs10_20000000L 
#make MCU_TARGET=atmega328p BOOT_ADR=0x7000 F_CPU=20000000 SD_CS_PORT=PORTC SD_CS_DDR=DDRC SD_CS_BIT=2 USE_LED=1 USE_UART=0 ADDED_CFLAGS="-Wextra -Wno-strict-aliasing" TARGET=atmega328p_cs16_20000000L 

make clean 
rm *.elf
avr-size *.hex > bootloader_size.txt
