gpio_expander library
=====================

 Universal GPIO library for Arduinos and Teensy 2/3/3.x
 
--------------------------------------------------------------------------------------

An attempt to create an universal library that works with many common GPIO chips and can be used with Arduino's and Teensy's (my preferred choice)<br>
Here's a list of the GPIO's chip managed:<br>

<table>
<tr>
<td><b>chip</b></td><td><b>company</b></td><td><b>pins</b></td><td><b>protocol</b></td><td><b>features</b></td><td><b>status</b></td>
</tr>
<tr>
<td><b>mcp23s17</b></td><td>Microchip</td><td>16</td><td>SPI</td><td>HAEN/INT</td><td>done</td>
</tr>
<tr>
<td><b>mcp23017</b></td><td>Microchip</td><td>16</td><td>I2C</td><td>INT</td><td>done</td>
</tr>
<tr>
<td><b>mcp2308</b></td><td>Microchip</td><td>8</td><td>I2C</td><td>INT</td><td>done</td>
</tr>
<tr>
<td><b>mcp23s08</b></td><td>Microchip</td><td>8</td><td>SPI</td><td>HAEN/INT</td><td>done</td>
</tr>
<tr>
<td><b>mcp23s18</b></td><td>Microchip</td><td>16</td><td>SPI</td><td>HAEN/INT</td><td>done</td>
</tr>
<tr>
<td><b>mcp23018</b></td><td>Microchip</td><td>16</td><td>I2C</td><td>INT</td><td>done</td>
</tr>
<tr>
<td><b>mcp23016</b></td><td>Microchip</td><td>16</td><td>I2C</td><td>INT</td><td>done</td>
</tr>
<tr>
<td><b>pcf8574</b></td><td>NTX</td><td>8</td><td>I2C</td><td>INT</td><td>dev</td>
</tr>
<tr>
<td><b>pcf8574a</b></td><td>NTX</td><td>8</td><td>I2C</td><td>INT/different address range</td><td>dev</td>
</tr>
<tr>
<td><b>pcf8575</b></td><td>NTX</td><td>8</td><td>I2C</td><td>INT</td><td>planned</td>
</tr>
<tr>
<td><b>pca9698</b></td><td>NTX</td><td>40</td><td>I2C</td><td>INT/64adrs</td><td>planned</td>
</tr>
<tr>
<td><b>pca9555</b></td><td>NTX</td><td>16</td><td>I2C</td><td>INT/64adrs</td><td>testing</td>
</tr>
<tr>
<td><b>tca9555</b></td><td>TI</td><td>16</td><td>I2C</td><td>INT/64adrs</td><td>testing</td>
</tr>
<tr>
<td><b>max6957</b></td><td>TI</td><td>20/28</td><td>I2C</td><td>INT/64adrs</td><td>testing</td>
</tr>
<tr>
<td><b>max7301</b></td><td>TI</td><td>20/28</td><td>I2C</td><td>INT/64adrs</td><td>testing</td>
</tr>
<tr>
<td><b>max7311</b></td><td>TI</td><td>16</td><td>I2C</td><td>INT/64adrs/hot insertion</td><td>testing</td>
</tr>
<tr>
<td><b>max7318</b></td><td>TI</td><td>16</td><td>I2C</td><td>INT/64adrs/hot insertion</td><td>testing</td>
</tr>
<tr>
<td><b>rd1073</b></td><td>LATTICE</td><td>16</td><td>SPI</td><td>INT</td><td>planned</td>
</tr>
<tr>
<td><b>xra1200</b></td><td>EXAR</td><td>8</td><td>I2C</td><td>INT</td><td>planned</td>
</tr>
<tr>
<td><b>xra1201</b></td><td>EXAR</td><td>16</td><td>I2C</td><td>INT</td><td>planned</td>
</tr>
<tr>
<td><b>xra1405</b></td><td>EXAR</td><td>16</td><td>SPI</td><td>INT/24Mhz SPI</td><td>dev</td>
</tr>
<tr>
<td><b>fxl6408</b></td><td>FAIRCHILD</td><td>8</td><td>I2C</td><td>INT</td><td>planned</td>
</tr>
<tr>
<td><b>bu1852guw</b></td><td>ROHM</td><td>20</td><td>I2C</td><td>INT</td><td>planned</td>
</tr>
<tr>
<td><b>cy8C9560</b></td><td>CYPRESS</td><td>60</td><td>I2C</td><td>INT/EEPROM/max 100Khz I2C</td><td>planned</td>
</tr>
<tr>
<td><b>sx1505</b></td><td>SEMTECH</td><td>8</td><td>I2C</td><td>INT/Indipendent rails</td><td>dev</td>
</tr>
<tr>
<td><b>sx1506</b></td><td>SEMTECH</td><td>16</td><td>I2C</td><td>INT/Indipendent rails</td><td>dev</td>
</tr>
<tr>
<td><b>sx1509</b></td><td>SEMTECH</td><td>16</td><td>I2C</td><td>INT/LED driver/Level Shifter</td><td>dev</td>
</tr>
</table>


<b>Status Legend:</b></br>
<dl>
<dt>done:</dt>
<dd>fully working.</dd>
<dt>dev:</dt>
<dd>currently in development.</dd>
<dt>testing:</dt>
<dd>currently in testing, may works or partially.</dd>
<dt>planned:</dt>
<dd>will be developed soon and added before the release.</dd>
<dt>planned[R2]:</dt>
<dd>will not included in the release but planned for version 2.</dd>
</dl>

--------------------------------------------------------------------------------------

When working with microcontrollers it's common run quickly out of I/O's, that because these small chips have few
pins and some periferal (like LCD) uses many pins. For this reason I use a lot external GPIO's chips.<br><br>
Microchip series MCP23xxx GPIO expanders are great chips but they can be a nightmare if you want to deal with registers, timings, etc., for these reasons a lot of coders prefere the supereasy to use PCF series from NTX.<br> 
I personally prefere the Microchip series because I can choose from I2C and SPI and they have an extra feature called <b>HAEN</b> that allow me to <i>share CS pin between 8 different chips in SPI mode!</i><br>
This library it's an improvement of my old MCP23xxx library since it's smaller, faster and deal with all MCP23xx series plus PCF series from NTX.<br>
I designed as a small and easy library that can be easily expanded but still have the ability to handle directly the registers for special purposes.<br>
If you need a simple expander, use this library and choose between many GPIO's chip by using really simply commands, but if you need a specialized GPIO you can still use this library and use the direct access to chip registers (see the keypad demo).<br>
The MCP23xxx registers are stored in each chip library extension so you can share demos between different chips (there's some exceptions like the MCP23016 that has limited features and you cannot share some features between NTX and MCP ones since they are internally very different).<br>
Library is modular, every chip it's an extension of the same library so it's easy to expand.<br>
Why create an unified library? It's better create single libraries for chip families? Seems no!<br>Almost all GPIO chips have really similar programming, only Microchip families has more features and looks more complicated but the access it's almost the same for all so it have sense to build up an unified way to access these chips with common commands so you can easily use the chip you like without have to recode anithing, just be sure you don't need some special features of some particular chip.<br>

 How To Use:
 
 Library it's unified so you just need toi include the chip and his protocol, example for <b>mcp23017</b>
 
```
 #include <Wire.h>
 #include <mcp23017.h>
```
 then you need an instance:
 
```
 mcp23017 gpio(address);//instance
 // NOTE: If you need to include library in other libraries you can use mcp23017 gpio(); (empty address)
 // then call postSetup!!
```
 where the address it's specific to the chip (read chip.h file)
 
 From below the commands are the same for all chip with some exceptions:
  - Some chip it's 8 bit so it doesn't have the <b>writeWord</b> command
  - Some chip has limited features so not all registers are accessible, take a look to the chip.h file)
 
 Commands are:

```
 gpio.postSetup(depends of chip);//Useful when you want include in other libraries
 gpio.begin();//gpio.begin(1); will not init the protocol (useful when multiple chip are used or you want to manually init it
 
 gpio.gpioPinMode(INPUT or OUTPUT or data);//set all pin accordly
 gpio.gpioPinMode(pin,mode);//set an individual pin as INPUT or OUTPUT
 gpio.port(data or HIGH or LOW);//data=8..xx bits, depends of chip. set all pins at once
 gpio.port(low byte,high byte);//useful for 16 bit GPIOs that have 2 ports of 8 bit
 gpio.readGpioPort();//read the state of all pins, it returns 8...xx bits depending of chip
 gpio.readGpioPortFast();//experimental. Read the state of the library buffer instead of the chip.
 gpio.gpioDigitalWrite(pin,state);//set individual pin HIGH or LOW
 gpio.gpioDigitalRead(pin);//read the state of one pin
 gpio.gpioDigitalReadFast(pin);//experimental. Read the state of a pin from the library buffer
 gpio.gpioRegisterReadByte(register);//not available to all chip, read a byte from the specific register
 gpio.gpioRegisterReadWord(register);//not available to all chip, read a word from the specific register
 gpio.gpioRegisterWriteByte(register,data);//write byte directly in chip register
 gpio.gpioRegisterWriteWord(register,data);//write word directly in chip register(not 8 bit chip)
 gpio.portPullup(HIGH or LOW or data);//Set pullup on input pin (not all chip!)

```
 
Another example: How to include in another library?

Here's how to inlude and use this library inside an existing one.
In the .h file of your existing library add this lines just after the aruino.h include or library protection declaration:

```
#ifndef _YOURLIB_H_
#define _YOURLIB_H_

#include <inttypes.h>

#include "yourlib.h"

//here start the include
#include <../SPI/SPI.h>//this chip needs SPI
#include <../gpio_expander/mcp23s17.h>
//here ends
```

 Now you have to provide an instance! Go to the private section of your library .h file and add to existings:
 
```
 private:
    bla
    bla
    
    uint8_t 		_cs;//needed for this chip
    uint8_t 		_adrs;//needed for this chip (if you using HAEN)

   mcp23s17		mygpio;//here the instance
```

	 As you noticed I have included also the _cs pin var and _adrs var needed for the chip I choose for demo.
	 Now open the .cpp file of your library and add these lines to your initialization function:
	 
	 
```
	 void myLibrary::begin(bla, bla){
	     bla
	     bla
	     
	      mygpio.postSetup(_cs,_adrs);//you should set these 2 vars before!
	      mygpio.begin();//put true if you want to init SPI (or I2C, depends of chip) manually!
	      mygpio.gpioPinMode(OUTPUT);
	      mygpio.gpioPort(0xFFFF);
	      etc...
	 }
```
  
  Since the library was already instanced and inited you can use the library function inside your library where you want but not forget to initialize it as I show above!<br>
  
<b>NEW! SUPPORT FOR ENERGIA IDE</b> - still not tested -<br>
<b>NEW! SUPPORT FOR SPI TRANSACTION</b>

Now library offer a basic support for the upcoming <b>SPI transactions</b>!

http://dorkbotpdx.org/blog/paul/spi_transactions_in_arduino

If the SPI library support SPI transactions it's possible to force this library to use it by instantiate like this:
```
	mcp23s17 gpio(cspin, address, SPI-SPEED);//For SPI transaction
```
The new parameter SPI-SPEED it's in Megahertz and mean the desidered SPI speed you want for communicate with the chip selected.
That parameter change from chip to chip and CPU used, ouch! The library has a rudimentary check control
for the max speed in relation to chip/CPU so if you put a too high this will reverted to the max supported speed by
chip and CPU.
If you are using this library inside another one by default the library will automatically use SPI transactions at max
speed supported by chip/CPU but you can still change this by call another NEW command:

```
	mcp23s17 gpio();//instance 
	gpio.setSPIspeed(24000000);//call this before postSetup!This will set the SPI to 24Mhz (Teensy3)
```
Setting setSPIspeed to 0 will force library to use standard SPI.

<b>ATTENTION!</b>
Setting SPI speed must be do consciously, you need to know exact if your CPU support that particular speed!
Arduino has <b>SPI_CLOCK_DIVx</b> that (if I'm right) divide the main clock speed for a x (2,4,8,16,etc. on UNO), DUE has similar approach, Teensy has some strain since it's not strictly related to CPU clock (change to 72Mhz to 98Mhz will not change SPI speed).
  
version <b>0.8b7</b> - beta release - only some driver released and partially tested!!!<br><br>
coded by Max MC Costa for s.u.m.o.t.o.y [sumotoy(at)gmail.com]

--------------------------------------------------------------------------------------
