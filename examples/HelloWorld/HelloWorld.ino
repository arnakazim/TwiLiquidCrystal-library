/*
  TwiLiquidCrystal Library - Hello World

  Demonstrates the basic use of a 16x2 LCD display over TWI (I2C).
  The TwiLiquidCrystal library works with all LCD displays that are
  compatible with the Hitachi HD44780 driver using an I2C backpack.

  This sketch prints "Hello World!" to the LCD and shows the time
  spent since the boot up of the Arduino. This is the exact same
  Hello World example as for the LiquidCrystal default library to
  demonstate the compatibilty between both libraries.

  The circuit:
  * LCD backpack SDA pin to analog pin A4 (on Nano)
  * LCD backpack SDL pin to analog pin A5 (on Nano)
  * LCD VCC pin to 5V
  * LCD GND pin to GND

  Library created in February 2021
  by Arnaud Ouvrier (http://www.arnaudouvrier.fr)

  This example code is in the public domain.

  https://github.com/arnakazim/TwiLiquidCrystal-library

*/

// include the library code:
#include <TwiLiquidCrystal.h>

// initialize the library by setting the I2C write address of the
// LCD backpack
TwiLiquidCrystal lcd(0x27);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}