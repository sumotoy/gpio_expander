gpio_expander library
=====================

 Universal GPIO library for Arduinos and Teensy 2/3/3.x
 
--------------------------------------------------------------------------------------

An attempt to create an universal library that works with many common GPIO chips:

- model -------- company -- pins -- protocol - Special Features

--------------------------------------------------------------------------------------

- mcp23s17 -- Microchip -- 16 ----- SPI ------- HAEN/INT
- mcp23017 -- Microchip -- 16 ----- I2C ------- INT
- mcp2308 ---- Microchip --- 8  ----- I2C ------- INT
- mcp23s08 --	Microchip --- 8  ----- SPI ------- HAEN/INT
- mcp23s18 -- Microchip -- 16 ----- SPI ------- HAEN/INT
- mcp23018 -- Microchip -- 16 ----- I2C ------- INT
- mcp23016 -- Microchip -- 16 ----- I2C ------- INT
- pcf8574 ------ NTX --------- 8  ------ I2C ------- INT
- pcf8574ap --- NTX --------- 8  ------ I2C ------- INT

--------------------------------------------------------------------------------------

Microchip series MCP23xxx GPIO expanders are great chips but they can be a nightmare
if you want to deal with registers, timings, etc., for these reasons a lot of coders
prefere the supereasy to use PCF series from NTX. I personally prefere the Microchip series
because I can choose from I2C and SPI and they have an extra feature called HAEN that
allow me to share CS pin between 8 different chips in SPI mode!
This library it's an improvement of my old MCP23xxx library since it's smaller, faster
and deal with all MCP23xx series plus PCF from NTX. 
The purpose is a small and easy library that can be easily expanded but still have the ability
to handle directly the registers.
If you need a simple expander, use this library and choose between many GPIO's chip by using
really simply commands, but if you need a specialized GPIO you can still use this library
and use the direct access to chip registers (see the keypad demo).
The MCP23xxx registers are stored in each chip library extension so you can share demos
between different chips (there's some exceptions like the MCP23016 that has limited features
and you cannot share some features between NTX and MCP ones since they are internally very different).
Library it's modular, every chip it's an extension of the same library so it's easy to expand.

version 0.5b1 - beta release - still NOT TESTED, just writed up.
coded by Max MC Costa for s.u.m.o.t.o.y [sumotoy(at)gmail.com]

--------------------------------------------------------------------------------------
