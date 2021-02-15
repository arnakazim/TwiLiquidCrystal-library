#ifndef TwiLiquidCrystal_h
#define TwiLiquidCrystal_h

#include "Arduino.h"
#include "Wire.h"

/*
 * Control bit defintions
 */

#define BL_BIT  0b00001000  // Backlight bit
#define EN_BIT  0b00000100  // Enable bit
#define RW_BIT  0b00000010  // Read/Write bit
#define RS_BIT  0b00000001  // Register select bit

#define DATA_PORTION 0b11110000 // D7 D6 D5 D4
#define CTRL_PORTION 0b00001111 // BL EN RW RS

/*
 * Command definitions
 * 
 * LCD_CLEARDISPLAY
 * LCD_RETURNHOME
 * LCD_ENTRYMODESET
 *   I/D = 1 -> Increment by 1
 *   S =   0 -> No shift
 * LCD_DISPLAYCONTROL
 *   D =   0 -> Display off
 *   C =   0 -> Cursor off
 *   B =   0 -> Blinking off
 * LCD_CURSORSHIFT
 *   S/C = 1 -> Shift cursor (0 for screen)
 *   R/L = 0 -> To the right
 * LCD_FUNCTIONSET
 *   DL =  1 -> 8-bit interface data
 *   N =   0 -> 1-line display
 *   F =   0 -> 5 × 8 dot character font
 * LCD_SETCGRAMADDR
 * LCD_SETDDRAMADDR
 */

#define LCD_CLEARDISPLAY    0b00000001 // 0   0   0   0   0   0   0   1
#define LCD_RETURNHOME      0b00000010 // 0   0   0   0   0   0   1   -
#define LCD_ENTRYMODESET    0b00000100 // 0   0   0   0   0   1 I/D   S
#define LCD_DISPLAYCONTROL  0b00001000 // 0   0   0   0   1   D   C   B
#define LCD_CURSORSHIFT     0b00010000 // 0   0   0   1 S/C R/L   -   -
#define LCD_FUNCTIONSET     0b00100000 // 0   0   1  DL   N   F   -   -
#define LCD_SETCGRAMADDR    0b01000000 // 0   1 ACG ACG ACG ACG ACG ACG
#define LCD_SETDDRAMADDR    0b10000000 // 1 ADD ADD ADD ADD ADD ADD ADD

class TwiLiquidCrystal
{
private:
    /*
    * Registers to store settings
    */
    uint8_t _ctrlRegister = 0b00000000; // To store BL EN RS RW bits state
    uint8_t _dsplRegister = 0b00000000; // To store the display settings
    uint8_t _modeRegister = 0b00000000; // To store the mode settings
    uint8_t _fctnRegister = 0b00000000; // To store the function settings

    /*
    * LCD settings
    */
    uint8_t _i2cLcdAddress;
    uint8_t _cols;
    uint8_t _rows;
    uint8_t _font;
    
    void set4BitMode();
public:
    TwiLiquidCrystal(uint8_t address, uint8_t cols, uint8_t rows, uint8_t font);
    void write(uint8_t data);
    void writeQuartet(uint8_t data);
    void setCtrlRegisterBit(uint8_t bit, bool state);
    void writeCmd(uint8_t data);
    void print(char* string);
    void setBacklight(bool state);
    void setFctnRegister(uint8_t bytemode, uint8_t lines, uint8_t font);
    void setDsplControl(uint8_t display, uint8_t cursor, uint8_t blink);
    void setEntryMode(uint8_t increment, uint8_t shift);
    void setCursor(uint8_t col, uint8_t row);
    void clear();
    void home();
    void begin();
};

#endif