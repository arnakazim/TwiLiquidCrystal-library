/*
  TwiLiquidCrystal Library - Basic example

  Demonstrates the basic use of a 16x2 LCD display over TWI (I2C).
  The TwiLiquidCrystal library works with all LCD displays that are
  compatible with the Hitachi HD44780 driver using an I2C backpack.

  This sketch demonstrates the use of this library by using multiple
  functionnalities like printing a custom char, text, turning on and
  off the backlight and moving the screen.

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

#include <TwiLiquidCrystal.h>

#define LCD_ADDRESS 0x27

uint8_t checkboardChar[] = {
  0b10101,
  0b01010,
  0b10101,
  0b01010,
  0b10101,
  0b01010,
  0b10101,
  0b01010
};

// Create an LCD object and setting the I2C address
TwiLiquidCrystal lcd(LCD_ADDRESS);

void setup() {
  // Initialize the LCD and setting up its size (16 colums by 2 rows) 
  lcd.begin(16, 2);

  // Adding the custom char to the LCD memory position 1
  lcd.createChar(1, checkboardChar);

  // Clear the LCD and setting the cursor at position 0, 0
  lcd.clear();
  lcd.home();
}

void loop() {
  lcd.cursor();
  lcd.blink();

  lcd.print("Hello, World!");
  delay(2000);

  lcd.noCursor();
  lcd.noBlink();

  // Turn ON  backlight
  lcd.backlight();
  delay(1000);

  // Turn OFF backlight
  lcd.noBacklight();
  delay(1000);

  // Move the cursor to the second line, 4th column
  lcd.setCursor(3, 1);
  lcd.print("Hello, World!");
  delay(1000);
  
  lcd.setCursor(16, 0);
  lcd.print((char)0x01); // Print the custom char

  lcd.setCursor(16, 1);
  lcd.print((char)0x01); // Print the custom char

  delay(1000);

  for(int i = 0; i < 17; i++) {
    lcd.scrollDisplayLeft();
    delay(250);
  }

  delay(1000);
  lcd.clear();
}
