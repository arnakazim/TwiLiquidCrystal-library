/*
  LiquidCrystal Library - Serial Input

  Demonstrates the basic use of a 16x2 LCD display over TWI (I2C).
  The TwiLiquidCrystal library works with all LCD displays that are
  compatible with the Hitachi HD44780 driver using an I2C backpack.

  This sketch displays text sent over the serial port
  (e.g. from the Serial Monitor) on an attached LCD. This is the exact
  same Serial Input example as for the LiquidCrystal default library to
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

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
TwiLiquidCrystal lcd(0x27);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
}

void loop() {
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
}