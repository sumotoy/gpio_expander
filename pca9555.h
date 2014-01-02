/*
                               _                
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
Universal GPIO library for Arduinos and Teensy 2/3/3.x
--------------------------------------------------------------------------------------
An attempt to create an universal library that works with many common GPIO chips:
model:			company:		pins:		protocol:		Special Features:
--------------------------------------------------------------------------------------
mcp23s17		Microchip		16			SPI					HAEN/INT
mcp23017		Microchip		16			I2C					INT
mcp2308			Microchip		8			I2C					INT
mcp23s08		Microchip		8			SPI					HAEN/INT
mcp23s18		Microchip		16			SPI					HAEN/INT
mcp23018		Microchip		16			I2C					INT
mcp23016		Microchip		16			I2C					INT
pcf8574			NTX				8			I2C					INT
pcf8574ap		NTX				8			I2C					INT
max7318			MAXIM			16			I2C					INT/Hot Insert Protection/64 addresses
TCA9555			TI			    16			I2C					INT/64 addresses/(uses same driver as max7318/pca9555)
PCA9555			NTX			    16			I2C					INT/64 addresses/(uses same driver as max7318/tca9555)
--------------------------------------------------------------------------------------
max7318 (PCA9555-TCA9555) Driver
version 0.5b3
--------------------------------------------------------------------------------------
Version history:
0.5b1: first release, just coded and never tested
0.5b2: fixed 2wire version, added portPullup, tested output mode (ok)
0.5b3: added some drivers
--------------------------------------------------------------------------------------
coded by Max MC Costa for s.u.m.o.t.o.y [sumotoy(at)gmail.com]
--------------------------------------------------------------------------------------
*/

/* ------------------------------ MAX7318 WIRING ------------------------------------
0x20...0xDE

				__ __
		int   [|  U  |] +++++
		A1    [|     |] sda
		A2    [|     |] scl
		IO-0  [|     |] A0
		IO-1  [|     |] IO-15
		IO-2  [|     |] IO-14
		IO-3  [|     |] IO-13
		IO-4  [|     |] IO-12
		IO-5  [|     |] IO-11
		IO-6  [|     |] IO-10
		IO-7  [|     |] IO-9
		GND   [|_____|] IO-8

		
Basic Addressing: 0 0 1 0 0   A2 A1 A0

Advanced Addressing:
_______________________________________________
AD2 AD1 AD0 A6 A5 A4 A3 A2 A1 A0 ADDRESS (hex)
GND SCL GND 0  0  1  0  0  0  0  0x20
GND SCL V+  0  0  1  0  0  0  1  0x22
GND SDA GND 0  0  1  0  0  1  0  0x24
GND SDA V+  0  0  1  0  0  1  1  0x26
V+  SCL GND 0  0  1  0  1  0  0  0x28
V+  SCL V+  0  0  1  0  1  0  1  0x2A
V+  SDA GND 0  0  1  0  1  1  0  0x2C
V+  SDA V+  0  0  1  0  1  1  1  0x2E
GND SCL SCL 0  0  1  1  0  0  0  0x30
GND SCL SDA 0  0  1  1  0  0  1  0x32
GND SDA SCL 0  0  1  1  0  1  0  0x34
GND SDA SDA 0  0  1  1  0  1  1  0x36
V+  SCL SCL 0  0  1  1  1  0  0  0x38
V+  SCL SDA 0  0  1  1  1  0  1  0x3A
V+  SDA SCL 0  0  1  1  1  1  0  0x3C
V+  SDA SDA 0  0  1  1  1  1  1  0x3E
GND GND GND 0  1  0  0  0  0  0  0x40
GND GND V+  0  1  0  0  0  0  1  0x42
GND V+  GND 0  1  0  0  0  1  0  0x44
GND V+  V+  0  1  0  0  0  1  1  0x46
V+  GND GND 0  1  0  0  1  0  0  0x48
V+  GND V+  0  1  0  0  1  0  1  0x4A
V+  V+  GND 0  1  0  0  1  1  0  0x4C
V+  V+  V+  0  1  0  0  1  1  1  0x4E
GND GND SCL 0  1  0  1  0  0  0  0x50
GND GND SDA 0  1  0  1  0  0  1  0x52
GND V+  SCL 0  1  0  1  0  1  0  0x54
GND V+  SDA 0  1  0  1  0  1  1  0x56
V+  GND SCL 0  1  0  1  1  0  0  0x58
V+  GND SDA 0  1  0  1  1  0  1  0x5A
V+  V+  SCL 0  1  0  1  1  1  0  0x5C
V+  V+  SDA 0  1  0  1  1  1  1  0x5E
SCL SCL GND 1  0  1  0  0  0  0  0xA0
SCL SCL V+  1  0  1  0  0  0  1  0xA2
SCL SDA GND 1  0  1  0  0  1  0  0xA4
SCL SDA V+  1  0  1  0  0  1  1  0xA6
SDA SCL GND 1  0  1  0  1  0  0  0xA8
SDA SCL V+  1  0  1  0  1  0  1  0xAA
SDA SDA GND 1  0  1  0  1  1  0  0xAC
SDA SDA V+  1  0  1  0  1  1  1  0xAE
SCL SCL SCL 1  0  1  1  0  0  0  0xB0
SCL SCL SDA 1  0  1  1  0  0  1  0xB2
SCL SDA SCL 1  0  1  1  0  1  0  0xB4
SCL SDA SDA 1  0  1  1  0  1  1  0xB6
SDA SCL SCL 1  0  1  1  1  0  0  0xB8
SDA SCL SDA 1  0  1  1  1  0  1  0xBA
SDA SDA SCL 1  0  1  1  1  1  0  0xBC
SDA SDA SDA 1  0  1  1  1  1  1  0xBE
SCL GND GND 1  1  0  0  0  0  0  0xC0
SCL GND V+  1  1  0  0  0  0  1  0xC2
SCL V+  GND 1  1  0  0  0  1  0  0xC4
SCL V+  V+  1  1  0  0  0  1  1  0xC6
SDA GND GND 1  1  0  0  1  0  0  0xC8
SDA GND V+  1  1  0  0  1  0  1  0xCA
SDA V+  GND 1  1  0  0  1  1  0  0xCC
SDA V+  V+  1  1  0  0  1  1  1  0xCE
SCL GND SCL 1  1  0  1  0  0  0  0xD0
SCL GND SDA 1  1  0  1  0  0  1  0xD2
SCL V+  SCL 1  1  0  1  0  1  0  0xD4
SCL V+  SDA 1  1  0  1  0  1  1  0xD6
SDA GND SCL 1  1  0  1  1  0  0  0xD8
SDA GND SDA 1  1  0  1  1  0  1  0xDA
SDA V+  SCL 1  1  0  1  1  1  0  0xDC
SDA V+  SDA 1  1  0  1  1  1  1  0xDE
*/
#ifndef _PCA9555_H_
#define _PCA9555_H_

#include <inttypes.h>

#include "gpio_expander.h"


class pca9555 : public gpio_expander
{

public:
	pca9555(const uint8_t adrs);
	
	virtual void 	begin(bool protocolInitOverride=false); //protocolInitOverride=true	will not init the SPI	

	
	void 			gpioPinMode(bool mode);						//set all pins to INPUT or OUTPUT
	void 			gpioPinMode(uint8_t pin, bool mode);		//set a unique pin as IN(1) or OUT (0)
	void 			gpioPort(uint16_t value);					//write data to all pins
	void			gpioPort(byte lowByte, byte highByte);		//same as abowe but uses 2 separate bytes
	uint16_t 		readGpioPort();								//read the state of the pins (all)
	uint16_t 		readGpioPortFast();							
	
	void 			gpioDigitalWrite(uint8_t pin, bool value);  //write data to one pin
	int 			gpioDigitalRead(uint8_t pin);				//read data from one pin
	unsigned int 	gpioRegisterRead(byte reg);					//read a chip register
	int 			gpioDigitalReadFast(uint8_t pin);
	void 			gpioRegisterWrite(byte reg,byte data);		//write a chip register
	// direct access commands
    void			writeByte(byte addr, byte data);	
	void 			writeWord(byte addr, uint16_t data);
	uint16_t 		readAddress(byte addr);
	
	//------------------------- REGISTERS
	byte			IODIR;//GPCONF
	byte			GPPU;//GPO
	byte			GPIO;//GPI
	byte			IPOL;//GPPOL
	
private:
	uint8_t 		_adrs;
	uint16_t		_gpioDirection;
	uint16_t		_gpioState;
	bool			_error;
};
#endif