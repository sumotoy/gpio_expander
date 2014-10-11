/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	gpio_expander - An attemp to create a fast and universal library for drive many GPIO chips
	
model:			company:		pins:		protocol:		Special Features:
---------------------------------------------------------------------------------------------------------------------
mcp23s08		Microchip		  8			SPI					INT/HAEN
---------------------------------------------------------------------------------------------------------------------
Version history:
0.5b1: first release, just coded and never tested
0.5b2: fixed 2wire version, added portPullup, tested output mode (ok)
0.5b3: added some drivers
0.5b4: ability to include library inside other libraries.
0.5b7: Changed functionalities of some function.
0.6b1: Changed gpioRegisterRead to gpioRegisterReadByte. Added gpioRegisterReadWord (for some GPIO)
0.6b3: Added basic support for SPI transactions, small optimizations.
---------------------------------------------------------------------------------------------------------------------
		Copyright (c) 2013-2014, s.u.m.o.t.o.y [sumotoy(at)gmail.com]
---------------------------------------------------------------------------------------------------------------------

    gpio_expander Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    gpio_expander Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

	
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:0.6b3: Added basic support for SPI transactions, small optimizations.
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
#include <../SPI/SPI.h>//this chip needs SPI

//defining max SPI speed (not definitive and can change in relation to chip used)
#if defined (SPI_HAS_TRANSACTION)
#if defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3.0 or 3.1
#define MAXSPISPEED				32000000
#elif defined(ARDUINO) && defined(__arm__) && !defined(CORE_TEENSY)	//DUE	
#define MAXSPISPEED				24000000
#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) //uno and max		
#define MAXSPISPEED				8000000
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define MAXSPISPEED				2000000
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
#define MAXSPISPEED				8000000
#else
#define MAXSPISPEED				2000000
#endif
#endif

class mcp23s08 : public gpio_expander
{

public:
	mcp23s08(const uint8_t csPin,const uint8_t haenAdrs);
	mcp23s08(const uint8_t csPin,const uint8_t haenAdrs,uint32_t spispeed);//for SPI transactions
	mcp23s08();
	void			postSetup(const uint8_t csPin,const uint8_t haenAdrs);//used with other libraries only
	virtual void 	begin(bool protocolInitOverride=false); //protocolInitOverride=true	will not init the SPI	

	
	void 			gpioPinMode(uint8_t mode);						//set all pins to INPUT or OUTPUT
	void 			gpioPinMode(uint8_t pin, bool mode);		//set a unique pin as IN(1) or OUT (0)
	void 			gpioPort(uint8_t value);					//write data to all pins
	//void			gpioPort(byte lowByte, byte highByte);		//same as abowe but uses 2 separate bytes (not applicable to this chip)
	uint8_t 		readGpioPort();								//read the state of the pins (all)
	uint8_t 		readGpioPortFast();							
	
	void 			gpioDigitalWrite(uint8_t pin, bool value);  //write data to one pin
	int 			gpioDigitalRead(uint8_t pin);				//read data from one pin
	uint8_t		 	gpioRegisterReadByte(byte reg);					//read a byte from chip register
	int 			gpioDigitalReadFast(uint8_t pin);
	void 			gpioRegisterWriteByte(byte reg,byte data);		//write a chip register
	void			portPullup(uint8_t data);						// true=pullup, false=pulldown all pins
	// direct access commands
	uint8_t 		readAddress(byte addr);
	
	void			setSPIspeed(uint32_t spispeed);//for SPI transactions
	
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
	
	uint32_t		_spiTransactionsSpeed;//for SPI transactions
	
	uint8_t 		_useHaen;
	uint8_t 		_readCmd;
	uint8_t 		_writeCmd;
	void 			startSend(bool mode);
	void 			endSend();
	uint8_t			_gpioDirection;
	uint8_t			_gpioState;
    void			writeByte(byte addr, byte data);	
};
#endif