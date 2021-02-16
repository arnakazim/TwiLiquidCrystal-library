# Arnakazim's TwiLiquidCrystal Arduino Library

___A library to interface an HD44780 based LCD via I2C (or TWI in Arduino terms)___

## Why another I2C/TWI LCD display library for Arduino?

That's a good question with a simple answer: there is no need for another one. I am in the process on making a board around an ATMega328P and I am planning to use a HD44780 base 16x2 display over I2C.

I was using LiquidCrystal_I2C at first but it is not well maintained and is basically a  copy/paste of LiquidCrystal with a different write4bits() function. It is nice from far but the LiquidCrystal library isn't so perfect...

So I have retrieved the HD44780 datasheet, read from it and started writting my own library. It worked fine for what I needed but then I was "why not making it LiquidCrystal compatible?". And there I am.

So at the moment it is 90% compatible. Timings are revised improved too.

 **WARNING**: The library is working but this is a WIP. Not all function are implemented. Use it with caution (as it may contain bugs). (see comment at the end of TwiLiquidCrystal.h file for more information).
