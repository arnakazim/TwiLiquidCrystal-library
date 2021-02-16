#include "Arduino.h"
#include "Wire.h"
#include "TwiLiquidCrystal.h"

TwiLiquidCrystal::TwiLiquidCrystal(uint8_t address, uint8_t cols, uint8_t rows, uint8_t font) {
  _i2cLcdAddress = address;
  _cols = cols;
  _rows = rows;
  _font = font;

  // set rows start address
  // On a 16x2:
  //  line 1, screen 1 [0x00 ; 0x0F]
  //  line 2, screen 1 [0x40 ; 0x4F]
  //  line 1, screen 2 [0x10 ; 0x1F]
  //  line 2, screen 2 [0x50 ; 0x5F]
  // On a 20x4 (only one screen, HD44780 can only store 80 characters):
  // (Remark: on a 20x4, incrementing the cursor give this result:
  //   Line 1 -> Line 3 -> Line 2 -> Line 4)
  //  line 1, screen 1 [0x00 ; 0x13]
  //  line 2, screen 1 [0x40 ; 0x53]
  //  line 3, screen 1 [0x14 ; 0x27]
  //  line 4, screen 1 [0x54 ; 0x67]
  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);
}

void TwiLiquidCrystal::setRowOffsets(int row1, int row2, int row3, int row4) {
  _rowOffsets[row1, row2, row3, row4];
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

void TwiLiquidCrystal::setDsplRegisterBit(uint8_t bit, bool state) {
  if(state) _dsplRegister |= bit;
  else _dsplRegister &= ~bit;
}

void TwiLiquidCrystal::setEntryModeBit(uint8_t bit, bool state) {
  if(state) _modeRegister |= bit;
  else _modeRegister &= ~bit;
}

// Merge the command quartet with the control command (BL EN RW RS)
void TwiLiquidCrystal::writeQuartet(uint8_t data) {
  data |= _ctrlRegister;

  write(data);
  write(data | EN_BIT); // pulse enable
  delayMicroseconds(1);
  write(data);
  delayMicroseconds(42);
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

// Initialization routine to set the LCD to 4 bit mode
void TwiLiquidCrystal::initializationRoutine() {
  // Init in 8-bit
  // LiquidCrystal was outputing 0x30 4500µs - 0x30 4500µs 0x30 150µs
  // The datasheet for the HD44780  says 0x30 4100µs - 0x30 100µs 0x30 no delay...
  // (HD44780U datasheet, page 45)
  // It also may be optionnal, useful only when "the power supply conditions for correctly operating the internal reset circuit are not met"
  writeQuartet(LCD_FUNCTIONSET | LCD_FUNCTIONSET_DL_BIT);
  delayMicroseconds(4200);
  writeQuartet(LCD_FUNCTIONSET | LCD_FUNCTIONSET_DL_BIT); 
  delayMicroseconds(110);
  writeQuartet(LCD_FUNCTIONSET | LCD_FUNCTIONSET_DL_BIT); 

  // set in 4-bit mode (Function set)
  writeQuartet(LCD_FUNCTIONSET);
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
	if ( row > _rows ) {
		row = _rows - 1; // set row to max row if overflow
	}
	writeCmd(LCD_SETDDRAMADDR | (col + _rowOffsets[row]));
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
  initializationRoutine();

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

void TwiLiquidCrystal::createChar(uint8_t index, uint8_t character[]) {
    index &= 0x7; // 7 editable characters

    writeCmd(LCD_SETCGRAMADDR | (index << 3));
    _ctrlRegister |= RS_BIT; // Set register to DATA
    for (uint8_t i=0; i<8; i++) {
      writeCmd(character[i]);
    }
    _ctrlRegister &= ~RS_BIT; // Reset register to INSTRUCTION
}

void TwiLiquidCrystal::selectScreen(uint8_t index) {
  home();

  for (uint8_t i = 0; i < index * _cols; i++) {
    writeCmd(LCD_CURSORSHIFT | LCD_CURSORSHIFT_SC_BIT);
  }
}

void TwiLiquidCrystal::backlight() {
  setBacklight(true);
}

void TwiLiquidCrystal::noBacklight() {
  setBacklight(false);
}

void TwiLiquidCrystal::display() {
  setDsplRegisterBit(LCD_DISPLAYCONTROL_D_BIT, true);
  writeCmd(LCD_DISPLAYCONTROL | _dsplRegister);
}

void TwiLiquidCrystal::noDisplay() {
  setDsplRegisterBit(LCD_DISPLAYCONTROL_D_BIT, false);
  writeCmd(LCD_DISPLAYCONTROL | _dsplRegister);
}

void TwiLiquidCrystal::blink(){
  setDsplRegisterBit(LCD_DISPLAYCONTROL_B_BIT, true);
  writeCmd(LCD_DISPLAYCONTROL | _dsplRegister);
}
    
void TwiLiquidCrystal::noBlink() {
  setDsplRegisterBit(LCD_DISPLAYCONTROL_B_BIT, false);
  writeCmd(LCD_DISPLAYCONTROL | _dsplRegister);
}

void TwiLiquidCrystal::cursor() {
  setDsplRegisterBit(LCD_DISPLAYCONTROL_C_BIT, true);
  writeCmd(LCD_DISPLAYCONTROL | _dsplRegister);
}

void TwiLiquidCrystal::noCursor() {
  setDsplRegisterBit(LCD_DISPLAYCONTROL_C_BIT, false);
  writeCmd(LCD_DISPLAYCONTROL | _dsplRegister);
}

void TwiLiquidCrystal::leftToRight() {
  setEntryModeBit(LCD_ENTRYMODESET_ID_BIT, true);
  writeCmd(LCD_ENTRYMODESET | _modeRegister);
}

// This is for text that flows Right to Left
void TwiLiquidCrystal::rightToLeft() {
  setEntryModeBit(LCD_ENTRYMODESET_ID_BIT, false);
  writeCmd(LCD_ENTRYMODESET | _modeRegister);
}

void TwiLiquidCrystal::autoscroll(void) {
  setEntryModeBit(LCD_ENTRYMODESET_S_BIT, true);
  writeCmd(LCD_ENTRYMODESET | _modeRegister);
}

// This will 'left justify' text from the cursor
void TwiLiquidCrystal::noAutoscroll(void) {
 setEntryModeBit(LCD_ENTRYMODESET_S_BIT, false);
  writeCmd(LCD_ENTRYMODESET | _modeRegister);
}

void TwiLiquidCrystal::scrollDisplayLeft(void) {
  writeCmd(LCD_CURSORSHIFT | LCD_CURSORSHIFT_SC_BIT);
}

void TwiLiquidCrystal::scrollDisplayRight(void) {
  writeCmd(LCD_CURSORSHIFT | LCD_CURSORSHIFT_RL_BIT | LCD_CURSORSHIFT_SC_BIT);
}

void TwiLiquidCrystal::command(uint8_t value) {
  writeCmd(value);
}