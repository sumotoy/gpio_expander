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
MCP23S08 Driver
version 0.5b2
--------------------------------------------------------------------------------------
Version history:
0.5b1: first release, just coded and never tested
0.5b2: fixed 2wire version, added portPullup, tested output mode (ok)
--------------------------------------------------------------------------------------
coded by Max MC Costa for s.u.m.o.t.o.y [sumotoy(at)gmail.com]
--------------------------------------------------------------------------------------
*/

/* ------------------------------ MCP23S08 WIRING ------------------------------------
This chip has a very useful feature called HAEN that allow you to share the same CS pin trough
4 different addresses. Of course chip has to be Microchip and should be assigned to different addresses!

Basic Address:  001000 A1 A0 (from 0x20 to 0x23)
A1,A0 tied to ground = 0x20
				__ __
	->  sck   [|  U  |] ++++
	->  mosi  [|     |] IO-7
	<-  miso  [|     |] IO-6
		A1    [|     |] IO-5
		A0    [|     |] IO-4
 rst (con.+)  [|     |] IO-3
		cs    [|     |] IO-2
		int   [|     |] IO-1
		GND   [|_____|] IO-0
*/
#ifndef _MCP23S08_H_
#define _MCP23S08_H_

#include <inttypes.h>

#include "gpio_expander.h"


class mcp23s08 : public gpio_expander
{

public:
	mcp23s08(const uint8_t csPin,const uint8_t haenAdrs);

	virtual void 	begin(bool protocolInitOverride=false); //protocolInitOverride=true	will not init the SPI	

	
	void 			gpioPinMode(bool mode);						//set all pins to INPUT or OUTPUT
	void 			gpioPinMode(uint8_t pin, bool mode);		//set a unique pin as IN(1) or OUT (0)
	void 			gpioPort(uint8_t value);					//write data to all pins
	//void			gpioPort(byte lowByte, byte highByte);		//same as abowe but uses 2 separate bytes (not applicable to this chip)
	uint8_t 		readGpioPort();								//read the state of the pins (all)
	uint8_t 		readGpioPortFast();							
	
	void 			gpioDigitalWrite(uint8_t pin, bool value);  //write data to one pin
	int 			gpioDigitalRead(uint8_t pin);				//read data from one pin
	unsigned int 	gpioRegisterRead(byte reg);					//read a chip register
	int 			gpioDigitalReadFast(uint8_t pin);
	void 			gpioRegisterWrite(byte reg,byte data);		//write a chip register
	void			portPullup(bool data);						// true=pullup, false=pulldown all pins
	// direct access commands
    void			writeByte(byte addr, byte data);	
	//void 			writeWord(byte addr, uint16_t data);        // not applicable
	uint8_t 		readAddress(byte addr);
	
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
    uint8_t 		_cs;
	uint8_t 		_adrs;
	uint8_t 		_useHaen;
	uint8_t 		_readCmd;
	uint8_t 		_writeCmd;
	void 			startSend(bool mode);
	void 			endSend();
	uint8_t			_gpioDirection;
	uint8_t			_gpioState;
};
#endif