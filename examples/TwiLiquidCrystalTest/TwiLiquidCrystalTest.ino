#include <TwiLiquidCrystal.h>

TwiLiquidCrystal lcd = TwiLiquidCrystal(0x27, 16, 2, 0);

void setup() {
  // Initialize the LCD
  lcd.begin();
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

  delay(5000);
  lcd.clear();
}
