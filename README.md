gpio_expander library
=====================

 Universal GPIO library for Arduinos and Teensy 2/3/3.x
 
--------------------------------------------------------------------------------------

An attempt to create an universal library that works with many common GPIO chips and can be used with Arduino's and Teensy's (my preferred choice)<br>
Here's a list of the GPIO's chip managed:<br>

<b>----- model ----- company -- pins -- protocol ---- features ------------------------------------------------- dev state ------</b>

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
- <b>rd1073</b> ------- LATTICE --- 16 ------- SPI ------- INT ------------------------------------------------------ out/in dev

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

version <b>0.5b2</b> - beta release - only some driver released and partially tested!!!<br><br>
coded by Max MC Costa for s.u.m.o.t.o.y [sumotoy(at)gmail.com]

--------------------------------------------------------------------------------------
