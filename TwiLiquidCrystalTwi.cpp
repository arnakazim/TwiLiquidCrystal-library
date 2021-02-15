#include "Arduino.h"
#include "Wire.h"
#include "TwiLiquidCrystal.h"

TwiLiquidCrystal::TwiLiquidCrystal(uint8_t address, uint8_t cols, uint8_t rows, uint8_t font) {
  _i2cLcdAddress = address;
  _cols = cols;
  _rows = rows;
  _font = font;
}

// write a byte to the I2C bus
void TwiLiquidCrystal::write(uint8_t data) {
  Wire.beginTransmission(_i2cLcdAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// Set the state of a bit in the Control register
void TwiLiquidCrystal::setCtrlRegisterBit(uint8_t bit, bool state) {
  if(state) _ctrlRegister |= bit;
  else _ctrlRegister &= ~bit;
}

// Merge the command quartet with the control command (BL EN RW RS)
void TwiLiquidCrystal::writeQuartet(uint8_t data) {
  data |= _ctrlRegister;

  write(data);
  write(data | EN_BIT); // pulse enable
  delayMicroseconds(1);
  write(data);
  delayMicroseconds(40);
}

// Take a command byte and split it in two quarter (LCD in 4 bit mode)
void TwiLiquidCrystal::writeCmd(uint8_t data) {
  writeQuartet(data & DATA_PORTION);
  writeQuartet((data << 4) & DATA_PORTION);
}

// Print a string to LCD
void TwiLiquidCrystal::print(char* string) {
  _ctrlRegister |= RS_BIT; // Set register to DATA
  int i = 0;

  while (string[i] != '\0') {
    writeCmd(string[i]);
    i++;
  }

  _ctrlRegister &= ~RS_BIT; // Reset register to INSTRUCTION
}

// Start routine to set the LCD to 4 bit mode
void TwiLiquidCrystal::set4BitMode() {
  // Init in 8-bit
  writeQuartet(0x30);
  delayMicroseconds(4500); // wait min 4.1ms
  // second try
  writeQuartet(0x30);
  delayMicroseconds(4500); // wait min 4.1ms
  // third go!
  writeQuartet(0x30); 
  delayMicroseconds(150);

  // set in 4-bit mode (Function set)
  writeQuartet(0x20);
}

void TwiLiquidCrystal::setBacklight(bool state) {
  setCtrlRegisterBit(BL_BIT, state);
  write(_ctrlRegister);
}

// set the function register
// bytemode = 0 -> 4-bit mode; lines = 2 -> 2 lines; font = 0 -> 5x8 dots, 1 = 5x10
void TwiLiquidCrystal::setFctnRegister(uint8_t bytemode, uint8_t lines, uint8_t font) {
  _fctnRegister = 0 | (bytemode << 4) | ((lines & 0x2) << 2) | (font << 2);
  writeCmd(LCD_FUNCTIONSET | _fctnRegister);
}

void TwiLiquidCrystal::setDsplControl(uint8_t display, uint8_t cursor, uint8_t blink) {
  _dsplRegister = 0 | (display << 2) | (cursor << 1) | blink;
  writeCmd(LCD_DISPLAYCONTROL | _dsplRegister);
}

void TwiLiquidCrystal::setEntryMode(uint8_t increment, uint8_t shift) {
  _modeRegister = 0 | (increment << 1) | shift;
  writeCmd(LCD_ENTRYMODESET | _modeRegister);
}

void TwiLiquidCrystal::setCursor(uint8_t col, uint8_t row) {
  uint8_t rowOffsets[] = { 0x00, 0x40 };
	if ( row > _rows ) {
		row = _rows - 1; // set row to max row if overflow
	}
	writeCmd(LCD_SETDDRAMADDR | (col + rowOffsets[row]));
}

// Clear the display
void TwiLiquidCrystal::clear() {
  writeCmd(LCD_CLEARDISPLAY);
  delay(2);
}

// Set cursor to 0;0 position
void TwiLiquidCrystal::home() {
  writeCmd(LCD_RETURNHOME);
  delay(2);
}

void TwiLiquidCrystal::begin() {
  Wire.begin();
  delay(1000); // LCD power up time

  write(0x00); // clear data line
  set4BitMode();

  if (_rows > 1) {
    setFctnRegister(0, 2, 0);
  } else {
    setFctnRegister(0, 1, _font);
  }

  clear();
  setDsplControl(1, 0, 0);
  setEntryMode(1, 0);
  home();
}