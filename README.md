gpio_expander library
=====================

 Universal GPIO library for Arduinos and Teensy 2/3/3.x
 
--------------------------------------------------------------------------------------

An attempt to create an universal library that works with many common GPIO chips and can be used with Arduino's and Teensy's (my preferred choice)<br>
Here's a list of the GPIO's chip managed:<br>

<b>----- model ----- company -- pins -- protocol ---- features ------------------------------------------------- dev state </b>

--------------------------------------------------------------------------------------

- <b>mcp23s17</b> -- Microchip -- 16 ----- SPI ------- HAEN/INT ------------------------------------------------ done
- <b>mcp23017</b> -- Microchip -- 16 ----- I2C ------- INT --------------------------------------------------------- done
- <b>mcp2308</b>  ---  Microchip --- 8 ------ I2C ------- INT --------------------------------------------------------- done
- <b>mcp23s08</b> --	Microchip --- 8  ----- SPI ------- HAEN/INT ------------------------------------------------- done
- <b>mcp23s18</b> -- Microchip -- 16 ----- SPI ------- HAEN/INT ------------------------------------------------ done
- <b>mcp23018</b> -- Microchip -- 16 ----- I2C ------- INT --------------------------------------------------------- done
- <b>mcp23016</b> --  Microchip -- 16 ----- I2C ------- INT --------------------------------------------------------- done
- <b>pcf8574</b> ------ NTX --------- 8  ------ I2C ------- INT --------------------------------------------------------- dev
- <b>pcf8574a</b> ---- NTX --------- 8  ------ I2C ------- INT/same as 8574 with different addressing ----- dev
- <b>pcf8575</b>------- NTX -------- 16  ------ I2C ------- INT -------------------------------------------------------- planned
- <b>pca9698</b> ------ NTX -------- 40  ------ I2C ------- INT/64 address ---------------------------------------- planned
- <b>pca9555</b> ------ NTX -------- 16  ------ I2C ------- INT/64 address ---------------------------------------- out/in dev
- <b>tca9555</b> --------- TI --------- 16  ------ I2C ------- INT/64 address ---------------------------------------- out/in dev
- <b>max7311</b> ------ MAXIM ----- 16  ------ I2C ------- INT/Hot insertion protection/64 addressing ----- out/in dev
- <b>max7318</b> ------ MAXIM ----- 16  ------ I2C ------- INT/Hot insertion protection/64 addressing ----- out/in dev
- <b>max7301</b> ------ MAXIM ----- 20/28 -- SPI ------- INT ------------------------------------------------------ out/in dev
- <b>max6957</b> ------ MAXIM ----- 20/28 -- SPI ------- INT ------------------------------------------------------ out/in dev
- <b>rd1073</b> ------- LATTICE --- 16 ------- SPI ------- INT ------------------------------------------------------ planned
- <b>xra1405</b> ------ EXAR ------ 16 ------- SPI ------- INT/Hi Speed SPI -------------------------------------- planned
- <b>xra1200</b> ------ EXAR -------- 8 ------- I2C ------- INT -------------------------------------------------------- planned
- <b>xra1201</b> ------ EXAR -------- 16 ------ I2C ------- INT -------------------------------------------------------- planned
- <b>fxl6408</b> ------ FAIRCHILD -- 8 ------- I2C ------- INT -------------------------------------------------------- planned
- <b>bu1852guw</b> - ROHM ------- 20 ----- I2C ------- INT -------------------------------------------------------- planned
- <b>cy8C9560</b> --- CYPRESS --- 60 ----- I2C ------- INT/EEPROM/max 100Khz ------------------------ planned (R2)
- <b>sx1509</b> ------- SEMTECH --- 16 ----- I2C ------- INT/Led Driver/Level shifter ------------------------ dev
- <b>sx1505</b> ------- SEMTECH --- 8 ------ I2C ------- INT/indipendent rails ---------------------------------- dev
- <b>sx1506</b> ------- SEMTECH --- 16 ----- I2C ------- INT/indipendent rails ---------------------------------- dev

--------------------------------------------------------------------------------------
Status Legend:
- done: fully working.
- dev: currently in development.
- planned: will be added before 1.0 rel.
- out/in dev: outputs working, inputs not tested or in development, will be ready soon.
- planned (R2): will be added in release 2.0.


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
 ```
 where the address it's specific to the chip (read chip.h file)
 
 From below the commands are the same for all chip with some exceptions:
  - Some chip it's 8 bit so it doesn't have the <b>writeWord</b> command
  - Some chip has limited features so not all registers are accessible, take a look to the chip.h file)
 
 Commands are:

 ```
 gpio.begin();//gpio.begin(1); will not init the protocol (useful when multiple chip are used or you want to manually init it
 
 gpio.gpioPinMode(INPUT or OUTPUT);//set all pin accordly
 gpio.gpioPinMode(pin,mode);//set an individual pin as INPUT or OUTPUT
 gpio.port(data);//data=8..xx bits, depends of chip. set all pins at once
 gpio.port(low byte,high byte);//useful for 16 bit GPIOs that have 2 ports of 8 bit
 gpio.readGpioPort();//read the state of all pins, it returns 8...xx bits depending of chip
 gpio.readGpioPortFast();//experimental. Read the state of the library buffer instead of the chip.
 gpio.gpioDigitalWrite(pin,state);//set individual pin HIGH or LOW
 gpio.gpioDigitalRead(pin);//read the state of one pin
 gpio.gpioDigitalReadFast(pin);//experimental. Read the state of a pin from the library buffer
 gpio.gpioRegisterRead(register);//not available to all chip, read the specific register
 gpio.gpioRegisterWrite(register,data);//write directly a chip register
 gpio.writeByte(register,byte);
 gpio.writeWord(register,word);
```
 

version <b>0.5b2</b> - beta release - only some driver released and partially tested!!!<br><br>
coded by Max MC Costa for s.u.m.o.t.o.y [sumotoy(at)gmail.com]

--------------------------------------------------------------------------------------
