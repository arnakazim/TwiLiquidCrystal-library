#include <TwiLiquidCrystal.h>

TwiLiquidCrystal lcd = TwiLiquidCrystal(0x27, 16, 2, 0);

void setup() {
  // Initialize the LCD
  lcd.begin();
}

void loop() {
  // Turn on display, cursor and blink
  lcd.cursor();
  lcd.blink();

  lcd.print("Hello, World!");
  delay(2000);

  // Turn on display and turn off cursor and blink
  lcd.noCursor();
  lcd.noBlink();

  // Set cursor to home position (Col 0, Row 0)
  lcd.home();

  // Turn ON  backlight
  lcd.backlight();
  delay(1000);

  // Turn OFF backlight
  lcd.noBacklight();
  delay(1000);

  // Turn OFF the display
  lcd.noDisplay();
  delay(1000);

  // Turn ON the display
  lcd.display();
  delay(1000);

  // Move the cursor to the second line, 4th column
  lcd.setCursor(3, 1);
  lcd.print("Hello, World!");

  delay(5000);
  lcd.clear();
}
