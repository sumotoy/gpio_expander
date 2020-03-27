/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	gpio_expander - An attemp to create a fast and universal library for drive many GPIO chips
	
model:			company:		pins:		protocol:		Special Features:
---------------------------------------------------------------------------------------------------------------------
mcp23s18		Microchip		 16			SPI					INT/OPEN DRAIN
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
0.8b4: Support for SPI Transaction post setup
0.8b5: Support for 16bit spi transfer, fixed SPI init for this chip, fixed example, fixed wiring scheme
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

*/

/* ------------------------------ MCP23S18 WIRING ------------------------------------
Address:  00100000 always 0x20

				__ __
		GROUND[|  U  |] NC
		NC    [|     |] IOA-7
		IOB-0 [|     |] IOA-6
		IOB-1 [|     |] IOA-5
		IOB-2 [|     |] IOA-4
		IOB-3 [|     |] IOA-3
		IOB-4 [|     |] IOA-2
		IOB-5 [|     |] IOA-1
		IOB-6 [|     |] IOA-0
		IOB-7 [|     |] INT-A
		+++++ [|     |] INT-B
		CS    [|     |] NC
		SCLK  [|     |] RST (connect to +)
		MOSI  [|_____|] MISO
*/
#ifndef _MCP23S18_H_
#define _MCP23S18_H_

#include <inttypes.h>

#include "gpio_expander.h"
#include <SPI.h>//this chip needs SPI

#include "_utility/SPI.parameters.h"

class mcp23s18 : public gpio_expander
{

public:
	mcp23s18(const uint8_t csPin,const uint8_t haenAdrs=0x20);//any pin,adrs not important, hardware coded at 0x20
	mcp23s18(const uint8_t csPin,const uint8_t haenAdrs,uint32_t spispeed);//for SPI transactions
	mcp23s18();
	void			postSetup(const uint8_t csPin,const uint8_t haenAdrs=0x20,uint32_t spispeed=0);//used with other libraries only
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
	void 			gpioRegisterWriteByte(byte reg,byte data,bool both=false);		//write a byte in a chip register
	void 			gpioRegisterWriteWord(byte reg,word data);		//write a word in a chip register
	void			portPullup(uint16_t data);						// HIGH=all pullup, LOW=all pulldown,0xxxx=you choose witch
	void			gpioPortUpdate();
	// direct access commands
	uint16_t 		readAddress(byte addr);
	int 			getInterruptNumber(byte pin);
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
	uint16_t		_gpioDirection;
	uint16_t		_gpioState;
	void			writeByte(byte addr, byte data);	
	void 			writeWord(byte addr, uint16_t data);
};
#endif