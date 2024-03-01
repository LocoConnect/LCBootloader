/*-------------------------------------------------------------------------/
/  Stand-alone MMC boot loader  R0.01
/--------------------------------------------------------------------------/
/
/  Copyright (C) 2010, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/--------------------------------------------------------------------------/
/ Dec 6, 2010  R0.01  First release
/--------------------------------------------------------------------------/
/ This is a stand-alone MMC/SD boot loader for megaAVRs. It requires a 4KB
/ boot section for code, four GPIO pins for MMC/SD as shown in sch.jpg and
/ nothing else. To port the boot loader into your project, follow the
/ instruction sdescribed below.
/
/ 1. Setup the hardware. Attach a memory card socket to the any GPIO port
/    where you like. Select boot size at least 4KB for the boot loader with
/    BOOTSZ fuses and enable boot loader with BOOTRST fuse.
/
/ 2. Setup the software. Change the four port definitions in the asmfunc.S.
/    Change MCU_TARGET, BOOT_ADR and MCU_FREQ in the Makefile. The BOOT_ADR
/    is a BYTE address of boot section in the flash. Build the boot loader
/    and write it to the device with a programmer.
/
/ 3. Build the application program and output it in binary form instead of
/    hex format. Rename the file "app.bin" and put it into the memory card.
/
/ 4. Insert the card and turn the target power on. When the boot loader found
/    the application file, the file is written into the flash memory prior to
/    start the application program. On-board LED lights (if exist) during
/    the flash programming operation.
/
/-------------------------------------------------------------------------*/
#if BUILD_VLXVERSION
const char filename[15] = "VLXCONNECT.BIN\0"; 	// USE FILENAME FOR VLX BUILD VERSION
#else
const char filename[13] = "LCONNECT.BIN\0"; 	// USE DEFAULT FILENAME
#endif
#define BOOTLOADER_EEPROM_VERSION 0x12 //BIN 0001 0010 --> DECIMAL 01.02 --> Version 1.2
#define BOOTLOADER_EEPROM_VERSION_EEPROM_ADDRES 0x002E

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>
#include "pff/src/pff.h"

#if BOOT_ADR > 0xFFFF
#define PGM_READ_BYTE(x) pgm_read_byte_far(x)
#else
#define PGM_READ_BYTE(x) pgm_read_byte(x)
#endif

#if USE_UART
#include "uart/uart.h"
#endif

#if USE_LED
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PINB
#define LED PINB5

#define LED_G PINB1 //Arduino Pin 9
#define LED_R PINB2 //Arduino Pin 10

void flash_led(uint8_t count)
{
	while (count--) {
		LED_PORT |= _BV(LED_G);
		_delay_ms(200);
		LED_PORT &= ~_BV(LED_G);
		_delay_ms(200);
	}
}
#endif

void flash_erase(DWORD);		/* Erase a flash page (asmfunc.S) */
void flash_write(DWORD, const BYTE*);	/* Program a flash page (asmfunc.S) */
FATFS Fatfs;					// Petit-FatFs work area 
BYTE Buff[SPM_PAGESIZE];	        	// Page data buffer 

void UpdateBootloaderVersion()
{
	eeprom_busy_wait();
	uint16_t addr = BOOTLOADER_EEPROM_VERSION_EEPROM_ADDRES;
	uint16_t val = BOOTLOADER_EEPROM_VERSION;
	uint8_t readval = 0;

	_EEGET(readval, addr);
	if (readval != val)
	{
		_EEPUT(addr, val);
		_EEPUT(addr + 1, 0);
	}
}

static uint8_t pagecmp(const DWORD fa)
{
	UINT i;
	uint8_t b_flash, b_buff;
	for (i = 0; i < SPM_PAGESIZE; i++) {
		b_flash = PGM_READ_BYTE(fa + i);
		b_buff = Buff[i];
		if (b_flash != b_buff) {
#if USE_UART  //output first difference
			UART_puthex32(fa); UART_puts(PSTR(":"));
			UART_puthex(b_flash); UART_puts(PSTR(" "));
			UART_puthex(b_buff); UART_newline();
#endif
			return 1;
		}
	}
#if USE_UART  //output first difference
	UART_puthex32(fa); UART_puts(PSTR(":"));
	UART_puts(PSTR("=")); UART_newline();
#endif
	return 0;
}

void doFlash()
{
	DWORD fa;	/* Flash address */
	UINT br;	/* Bytes read */
	for (fa = 0; fa < BOOT_ADR; fa += SPM_PAGESIZE) {	/* Update all application pages */

		memset(Buff, 0xFF, SPM_PAGESIZE);		/* Clear buffer */
		pf_read(Buff, SPM_PAGESIZE, &br);		/* Load a page data */

		if (pagecmp(fa))
		{		/* Only flash if page is changed */
			flash_erase(fa);		/* Erase a page */
			flash_write(fa, Buff);		/* Write it if the data is available */
		}
	}
#if USE_UART
	UART_puts(PSTR("P"));
	UART_newline();
#endif
}

void checkFile()
{
	uint8_t fresult;

	fresult = pf_mount(&Fatfs);	/* Initialize file system */

	if (fresult != FR_OK)
	{ /* File System could not be mounted */
	#if USE_UART
		UART_puts(PSTR("M"));
		UART_newline();
	#endif		
	#if USE_LED
		flash_led(5);
	#endif		  
		return;
	}
	/*
		WORD flashver = eeprom_read_word((const uint16_t *)E2END - 1);
		if (flashver > 999) {
			flashver = 0;
		}
		BYTE y, tmp;
		WORD x;
		BYTE found = 0;

		for (x = flashver+10; x > flashver; x--) {
			y = x / 100;
			filename[5] = y + 0x30;
			tmp = x % 100;

			y = tmp / 10;
			filename[6] = y + 0x30;
			tmp = x % 10;

			filename[7] = tmp + 0x30;

			if (pf_open(filename) == FR_OK) { // File opens normally
				found = 1;
				doProgram();
			}
			led_power_toggle();
		}

		if (found == 0) {*/

	fresult = pf_open(filename);

	if (fresult != FR_OK)
	{ /* File could not be opened */
	#if USE_UART
		UART_puts(PSTR("O"));
		UART_newline();
	#endif
	#if USE_LED
		flash_led(10);
	#endif
		return;
	}
	doFlash();
}

int main(void)
{
	uint8_t ch = MCUSR;

	MCUSR = 0;
	WDTCSR |= _BV(WDCE) | _BV(WDE);
	WDTCSR = 0;

	// Check if the WDT was used to reset, in which case we dont bootload and skip straight to the code.
	if (!(ch & _BV(EXTRF))) // if its a not an external reset...
	{
		((void(*)(void))0)();	  //EXIT BOOTLOADER
	}
	else
	{
		/*
		Bootloader turns on Red led during boot
		 - flashes Green 5x when no SD card can be read
		 - flashes Green 10x when file cannot be read
		 - flashes Green 15x when cannot boot to program
		When BL finishes Red led will be off

		*/

		#if USE_LED
			//Set Led as output
			LED_DDR |= _BV(LED_G);
			//Set Red on
			LED_DDR |= _BV(LED_R);
			LED_PORT |= _BV(LED_R);
		#endif

		_delay_ms(50);

		#if USE_UART
			UART_init();
			UART_puts(PSTR("I"));
			UART_newline();
		#endif	

		UpdateBootloaderVersion();

		while (1)
		{
			checkFile();
			if (pgm_read_word(0) != 0xFFFF)
			{
				#if USE_UART
					UART_puts(PSTR("B"));
					UART_newline();
					_delay_ms(500);
				#endif
				#if USE_LED
					LED_PORT &= ~_BV(LED_G);
				#endif
				((void(*)(void))0)();	  //EXIT BOOTLOADER
			}

			#if USE_UART
				UART_puts(PSTR("R"));
				UART_newline();
			#endif
			#if USE_LED
				flash_led(15);
			#endif
			_delay_ms(5000);              // Retry		   
		}
	}
}
