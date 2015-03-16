/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	gpio_expander - An attemp to create a fast and universal library for drive many GPIO chips
	
model:			company:		pins:		protocol:		Special Features:
---------------------------------------------------------------------------------------------------------------------
PCF8574			NTX		  		8			I2C					INT
---------------------------------------------------------------------------------------------------------------------
Version history:
0.8b3: Added 2 more commands and 2 gpio chip.
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

/* ------------------------------ PCF8574/a WIRING ------------------------------------
Basic Address:  00100 A2 A1 A0 (from 0x20 to 0x27)
Basic Address:  00111 A2 A1 A0 (from 0x38 to 0x3F)A version
A2,A1,A0 tied to ground = 0x20 (pcf8574) or 0x38 (pcf8574a)
				__ __
		A0    [|  U  |] ++++
		A1    [|     |] sda <->
	    A2    [|     |] scl <->
		IO-0  [|     |] INT ->
		IO-1  [|     |] IO-7
		IO-2  [|     |] IO-6
		IO-3  [|     |] IO-5
		GND   [|_____|] IO-4
*/
#ifndef _PCF8574_H_
#define _PCF8574_H_

#include <inttypes.h>

#include "gpio_expander.h"


class pcf8574 : public gpio_expander
{

public:
	pcf8574(const uint8_t adrs);
	pcf8574();
	void			postSetup(const uint8_t adrs);//used with other libraries only
	virtual void 	begin(bool protocolInitOverride=false); //protocolInitOverride=true	will not init the SPI	

	
	void 			gpioPinMode(uint8_t mode);					//OUTPUT=all out,INPUT=all in,INPUT_PULLUP in,0xxxx=you choose
	void 			gpioPinMode(uint8_t pin, uint8_t mode);		//set a unique pin as INPUT,INPUT_PULLUP,OUTPUT
	void 			gpioPort(uint8_t value);					//HIGH=all Hi, LOW=all Low,0xxxx=you choose witch low or hi
	uint8_t 		readGpioPort();								//read the state of the pins (all)
	uint8_t 		readGpioPortFast();							//experimental! Works only when pins are outs!
	void			gpioDigitalWriteFast(uint8_t pin, bool value);
	void 			gpioDigitalWrite(uint8_t pin, bool value);  //write data to one pin
	uint8_t 		gpioDigitalRead(uint8_t pin);				//read data from one pin
	uint8_t 		gpioDigitalReadFast(uint8_t pin);           //experimental! Works only when pin are outs!
	void 			gpioPinToggle(uint8_t pin);
	void			gpioPortUpdate();
	
private:
	uint8_t 		_adrs;
	uint8_t			_gpioDirection;
	uint8_t			_gpioState;
	uint8_t			_gpioInputs;
	bool			_error;
	void			updateGpio();
	void			readGpio();
};
#endif