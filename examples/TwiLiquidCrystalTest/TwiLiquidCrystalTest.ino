#include <TwiLiquidCrystal.h>
#include "Print.h"

uint8_t customChar[] = {
  0b00000,
  0b00000,
  0b00100,
  0b00000,
  0b00001,
  0b00000,
  0b10101,
  0b00000
};

TwiLiquidCrystal lcd = TwiLiquidCrystal(0x27);

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2, 0);

  lcd.createChar(1, customChar);
  lcd.home();
}

void loop() {
  // Turn on display, cursor and blink
  lcd.setDsplControl(1, 1, 1);

  lcd.print("Hello, World!");
  delay(2000);

  // Turn on display and turn off cursor and blink
  lcd.setDsplControl(1, 0, 0);

  // Set cursor to home position (Col 0, Row 0)
  lcd.home();

  // Turn ON  backlight
  lcd.setBacklight(true);
  delay(1000);

  // Turn OFF backlight
  lcd.setBacklight(false);
  delay(1000);

  // Turn OFF the display
  lcd.setDsplControl(0, 0, 0);
  delay(1000);

  // Turn ON the display
  lcd.setDsplControl(1, 0, 0);
  delay(1000);

  // Move the cursor to the second line, 4th column
  lcd.setCursor(3, 1);
  lcd.print("Hello, World!");
  delay(1000);
  
  char txt[] = { 0x01, 0x00};
  lcd.setCursor(17, 0);
  lcd.print(txt);

  lcd.setCursor(17, 1);
  lcd.print(txt);

  lcd.selectScreen(1);
  delay(1000);

  lcd.selectScreen(0);
  delay(1000);

  for(int i = 0; i < 18; i++) {
    lcd.scrollDisplayLeft();
    delay(250);
  }

  lcd.clear();
  lcd.print(';)', HEX);

  delay(1000);

  lcd.clear();
}
