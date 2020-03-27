/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	gpio_expander - An attemp to create a fast and universal library for drive many GPIO chips
	
model:			company:		pins:		protocol:		Special Features:
---------------------------------------------------------------------------------------------------------------------
mcp23s17		Microchip		 16			SPI					INT/HAEN
---------------------------------------------------------------------------------------------------------------------
Version history:
0.5b1: first release, just coded and never tested
0.5b2: fixed 2wire version, added portPullup, tested output mode (ok)
0.5b3: added some drivers
0.5b4: ability to include library inside other libraries.
0.5b7: Changed functionalities of some function.
0.6b1: Changed gpioRegisterRead to gpioRegisterReadByte. Added gpioRegisterReadWord (for some GPIO)
0.6b3: Added basic support for SPI transactions, small optimizations.
0.8b3: Added 2 more commands and 2 gpio chip.
0.8b4: gpioRegisterWriteByte have an optional parameter to write in both banks
0.8b5: support for 16 bit spi transfer
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
	Version:0.8b3: Added 2 more commands and 2 gpio chip.
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/* ------------------------------ MCP23S17 WIRING ------------------------------------
This chip has a very useful feature called HAEN that allow you to share the same CS pin trough
8 different addresses. Of course chip has to be Microchip and should be assigned to different addresses!

Basic Address:  00100 A2 A1 A0 (from 0x20 to 0x27)
A2,A1,A0 tied to ground = 0x20
				__ __
		IOB-0 [|  U  |] IOA-7
		IOB-1 [|     |] IOA-6
		IOB-2 [|     |] IOA-5
		IOB-3 [|     |] IOA-4
		IOB-4 [|     |] IOA-3
		IOB-5 [|     |] IOA-2
		IOB-6 [|     |] IOA-1
		IOB-7 [|     |] IOA-0
		++++  [|     |] INT-A
		GND   [|     |] INT-B
		CS    [|     |] RST (connect to +)
		SCK   [|     |] A2
		MOSI  [|     |] A1
		MISO  [|_____|] A0
*/
#ifndef _MCP23S17_H_
#define _MCP23S17_H_

#include <inttypes.h>

#include "gpio_expander.h"
#include <SPI.h>//this chip needs SPI

#include "_utility/SPI.parameters.h"


/*   The IOCON register!
		 7		6     5	     4     3   2     1    0
IOCON = BANK MIRROR SEQOP DISSLW HAEN ODR INTPOL -NC-
-----------------------------------------------------------------------
0b01101100
BANK: (Controls how the registers are addressed)
1 The registers associated with each port are separated into different banks
0 The registers are in the same bank (addresses are sequential)
MIRROR: (INT Pins Mirror bit)
1 The INT pins are internally connected
0 The INT pins are not connected. INTA is associated with PortA and INTB is associated with PortB
SEQOP: (Sequential Operation mode bit)
1 Sequential operation disabled, address pointer does not increment
0 Sequential operation enabled, address pointer increments.
DISSLW: (Slew Rate control bit for SDA output, only I2C)
HAEN: (Hardware Address Enable bit, SPI only)
1 Enables the MCP23S17 address pins
0 Disables the MCP23S17 address pins
ODR: (This bit configures the INT pin as an open-drain output)
1 Open-drain output (overrides the INTPOL bit).
0 Active driver output (INTPOL bit sets the polarity).
INTPOL: (This bit sets the polarity of the INT output pin)
1 Active high
0 Active low
*/
class mcp23s17 : public gpio_expander
{

public:
	mcp23s17(const uint8_t csPin,const uint8_t haenAdrs);//any pin,0x20....0x27
	mcp23s17(const uint8_t csPin,const uint8_t haenAdrs,uint32_t spispeed);//for SPI transactions
	mcp23s17();//For include inside other libraries
	void 			postSetup(const uint8_t csPin,const uint8_t haenAdrs,uint32_t spispeed=0);//used with other libraries only
	virtual void 	begin(bool protocolInitOverride=false); //protocolInitOverride=true	will not init the SPI	
    
	void 			gpioPinMode(uint16_t mode);					//OUTPUT=all out,INPUT=all in,0xxxx=you choose
	void 			gpioPinMode(uint8_t pin, bool mode);		//set a unique pin as IN(1) or OUT (0)
	void 			gpioPort(uint16_t value);					//HIGH=all Hi, LOW=all Low,0xxxx=you choose witch low or hi
	void			gpioPort(byte lowByte, byte highByte);		//same as abowe but uses 2 separate bytes
	uint16_t 		readGpioPort();								//read the state of the pins (all)
	uint16_t 		readGpioPortFast();							
	
	void 			gpioDigitalWrite(uint8_t pin, bool value);  //write data to one pin
	void			gpioDigitalWriteFast(uint8_t pin, bool value);
	int 			gpioDigitalRead(uint8_t pin);				//read data from one pin
	uint8_t		 	gpioRegisterReadByte(byte reg);					//read a byte from chip register
	uint16_t		gpioRegisterReadWord(byte reg);					//read a word from chip register
	int 			gpioDigitalReadFast(uint8_t pin);
	void 			gpioRegisterWriteByte(byte reg,byte data,bool both=false);//write a byte in a chip register, optional for both ports
					//if both=true it will write the same register in bank A & B
	void 			gpioRegisterWriteWord(byte reg,word data);		//write a word in a chip register
	void			portPullup(uint16_t data);						// HIGH=all pullup, LOW=all pulldown,0xxxx=you choose witch
	void			gpioPortUpdate();
	// direct access commands
	uint16_t 		readAddress(byte addr);
	
	void			setSPIspeed(uint32_t spispeed);//for SPI transactions
	int 			getInterruptNumber(byte pin);
	
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
	uint16_t		_gpioDirection;
	uint16_t		_gpioState;
	void			writeByte(byte addr, byte data);	
	void 			writeWord(byte addr, uint16_t data);
};
#endif