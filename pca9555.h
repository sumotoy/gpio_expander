/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	gpio_expander - An attemp to create a fast and universal library for drive many GPIO chips
	
model:			company:		pins:		protocol:		Special Features:
---------------------------------------------------------------------------------------------------------------------
pca9555		      NTX		     16			I2C					INT/64 addresses
tca9555		       TI		     16			I2C					INT/64 addresses
---------------------------------------------------------------------------------------------------------------------
Version history:
0.8b4: Fixed an error for this chip
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

/* ------------------------------ PCA9555/TCA9555 WIRING ------------------------------------
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
	pca9555();;//used with other libraries only
	void			postSetup(const uint8_t adrs);//used with other libraries only
	
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
	int 			gpioDigitalReadFast(uint8_t pin);
	void 			gpioRegisterWriteByte(byte reg,byte data);		//write a byte in a chip register
	void 			gpioRegisterWriteWord(byte reg,word data);		//write a word in a chip register
	void			portPullup(uint16_t data);						// HIGH=all pullup, LOW=all pulldown,0xxxx=you choose witch
	void			gpioPortUpdate();
	// direct access commands
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
	void			writeByte(byte addr, byte data);	
	void 			writeWord(byte addr, uint16_t data);
};
#endif