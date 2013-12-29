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
--------------------------------------------------------------------------------------
mcp23018 Driver
version 0.5b1
coded by Max MC Costa for s.u.m.o.t.o.y [sumotoy(at)gmail.com]
--------------------------------------------------------------------------------------
*/

/* ------------------------------ MCP23018 WIRING ------------------------------------

Basic Address:  00100 A2 A1 A0 (from 0x20 to 0x27)
CAUTION! MCP23018 uses a resistor partition on adrs pin to determine address! 
Read carefully the datasheet for further information...
				__ __
		GND   [|  U  |] -NC-
		-NC-  [|     |] IOA-7
		IOB-0 [|     |] IOA-6
		IOB-1 [|     |] IOA-5
		IOB-2 [|     |] IOA-4
		IOB-3 [|     |] IOA-3
		IOB-4 [|     |] IOA-2
		IOB-5 [|     |] IOA-1
		IOB-6 [|     |] IOA-0
		IOB-7 [|     |] INT-A
		++++  [|     |] INT-B
		SCL   [|     |] -NC-
		SDA   [|     |] RST (connect to +)
		-NC-  [|_____|] adrs
*/
#ifndef _MCP23018_H_
#define _MCP23018_H_

#include <inttypes.h>

#include "gpio_expander.h"


class mcp23018 : public gpio_expander
{

public:
	mcp23018(const uint8_t adrs);

	virtual void 	begin(uint8_t protocolInitOverride=0); //protocolInitOverride=1	will not init the SPI	

	
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
	void 			writeWord(byte addr, word data);
	uint16_t 		readAddress(byte addr);
	
	//------------------------- REGISTERS
	byte			IOCON;
	byte			IODIR;
	byte			GPPU;
	byte			GPIO;
	byte			GPINTEN;
	byte			IPOL;
	byte			DEFVAL;
	byte			INTF;
	byte			INTCAP;
	byte			OLAT;
	byte			INTCON;
	
private:
	uint8_t 		_adrs;
	uint16_t			_gpioDirection;
	uint16_t			_gpioState;
};
#endif