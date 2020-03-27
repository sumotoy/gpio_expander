/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	gpio_expander - An attemp to create a fast and universal library for drive many GPIO chips
	
model:			company:		pins:		protocol:		Special Features:
---------------------------------------------------------------------------------------------------------------------
max6957		Maxim		 		20/28			SPI					INT
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

/* ------------------------------ max6957 WIRING ------------------------------------
This chip has a very useful feature called HAEN that allow you to share the same CS pin trough
8 different addresses. Of course chip has to be Microchip and should be assigned to different addresses!

Basic Address:  00100 A2 A1 A0 (from 0x20 to 0x27)
A2,A1,A0 tied to ground = 0x20

28PIN version
				__ __
		ISET  [|  U  |] +++++
		GND   [|     |] CS
		GND   [|     |] MISO
	    MOSI  [|     |] SCLK
	  IOB-12  [|     |] IOB-31
	  IOB-13  [|     |] IOB-30
	  IOB-14  [|     |] IOB-29
	  IOB-15  [|     |] IOB-28
	  IOB-16  [|     |] IOB-27
	  IOB-17  [|     |] IOB-26
	  IOB-18  [|     |] IOB-25
	  IOB-19  [|     |] IOB-24
	  IOB-20  [|     |] IOB-23
	  IOB-21  [|_____|] IOB-22
	
36PIN version	
  				__________
		ISET  [|     U    |] +++++
		GND   [|          |] CS
		GND   [|          |] MISO
	    MOSI  [|          |] SCLK
	  IOB-08  [|          |] IOB-04
	  IOB-12  [|          |] IOB-31
	  IOB-09  [|          |] IOB-05
	  IOB-13  [|          |] IOB-30
	  IOB-10  [|          |] IOB-06
	  IOB-14  [|          |] IOB-29
	  IOB-11  [|          |] IOB-07
	  IOB-15  [|          |] IOB-28
	  IOB-16  [|          |] IOB-27
	  IOB-17  [|          |] IOB-26
	  IOB-18  [|          |] IOB-25
	  IOB-19  [|          |] IOB-24
	  IOB-20  [|          |] IOB-23
	  IOB-21  [|__________|] IOB-22
*/
/*
COMMANDS
CONFIGURATION							0x04
TRANSITION DETECTION MASK				0x06
PORT CONFIGURATION P07,P06,P05,P04		0x09
PORT CONFIGURATION P11,P10,P09,P08		0x0A
PORT CONFIGURATION P15,P14,P13,P12		0x0B
PORT CONFIGURATION P19,P18,P17,P16		0x0C
PORT CONFIGURATION P23,P22,P21,P20		0x0D
PORT CONFIGURATION P27,P26,P25,P24		0x0E
PORT CONFIGURATION P31,P30,P29,P28		0x0F
MAX7301
4-Wire-Interfaced, 2.5V to 5.5V, 20-Port and
28-Port I/O Expander
______________________________________________________________________________________ 13
Table 4. Power-Up Configuration
REGISTER DATA	REGISTER FUNCTION	POWER-UP CONDITION	ADDRESS	CODE	(HEX)	D7 D6 D5 D4 D3 D2 D1 D0		Port Register	Bits 4 to 31	GPIO Output Low	0x24 to 0x3F
XXXXXXX
0
Configuration
Register
Shutdown Enabled
Transition Detection Disabled
0x04
00XX
XXX
0
Input Mask
Register
All Clear (Masked Off) 0x06
X000
000
0
Port
Configuration
P7, P6, P5, P4: GPIO Inputs Without Pullup 0x09
1010101
0
Port
Configuration
P11, P10, P9, P8: GPIO Inputs Without Pullup 0x0A
1010101
0
Port
Configuration
P15, P14, P13, P12: GPIO Inputs Without
Pullup
0x0B
1010101
0
Port
Configuration
P19, P18, P17, P16: GPIO Inputs Without
Pullup
0x0C
1010101
0
Port
Configuration
P23, P22, P21, P20: GPIO Inputs Without
Pullup
0x0D
1010101
0
Port
Configuration
P27, P26, P25, P24: GPIO Inputs Without
Pullup
0x0E
1010101
0
Port
Configuration
P31, P30, P29, P28: GPIO Inputs Without
Pullup
0x0F
1010101
0
X = Unused bits; if read, zero results.
Table 5. Configuration Register Format
REGISTER DATA
FUNCTION
ADDRESS CODE
(HEX)
D7 D6 D5 D4 D3 D2 D1 D0
Configuration Register
0x04 M 0
XXXXXS
Table 6. Shutdown Control (S Data Bit D0) Format
REGISTER DATA
FUNCTION
ADDRESS CODE
(HEX)
D7 D6 D5 D4 D3 D2 D1 D0
Shutdown 0x04 M 0
XXXXX0
Normal Operation 0x04 M 0
XXXXX

*/
#ifndef _MAX6957_H_
#define _MAX6957_H_

#include <inttypes.h>

#include "gpio_expander.h"
#include <SPI.h>//this chip needs SPI

#include "_utility/SPI.parameters.h"

class max6957 : public gpio_expander
{

public:
	max6957(const uint8_t csPin,const uint8_t haenAdrs);//any pin,0x20....0x27
	max6957(const uint8_t csPin,const uint8_t haenAdrs,uint32_t spispeed);//for SPI transactions
	max6957();//For include inside other libraries
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
	void 			gpioRegisterWriteByte(byte reg,byte data);		//write a byte in a chip register
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