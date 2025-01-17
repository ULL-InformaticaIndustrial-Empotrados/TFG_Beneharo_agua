#include "BBB_I2C_LCD.hpp"

#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <fstream>

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

BBB_I2C_LCD::BBB_I2C_LCD(uint8_t i2c_bus, uint8_t lcd_addr, uint8_t lcd_cols,
    uint8_t lcd_rows, uint8_t charsize) {
  _i2c_bus = i2c_bus;
  _addr = lcd_addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _charsize = charsize;
  _backlightval = LCD_BACKLIGHT;
}

void BBB_I2C_LCD::configurePinmuxI2C(std::string pinName) {

    // try to configure pinmux
  std::string pinmuxPath = "/sys/devices/platform/ocp/ocp:" + pinName
      + "_pinmux/state";
  std::string mode = "i2c";

  std::ofstream fMux(pinmuxPath);
  if (!fMux.good())
    throw std::invalid_argument("Unable to open pinmux file of "
        + pinName + ": " + pinmuxPath);
  fMux << mode << std::endl;
  if (!fMux.good())
    throw std::invalid_argument("Unable to set pinmux of "
        + pinName + " as '" + mode + "'");
  fMux.close();
}

void BBB_I2C_LCD::begin() {

  // Configurin pin-mux
  if (_i2c_bus == 1) {
    configurePinmuxI2C("P9_17");
    configurePinmuxI2C("P9_18");
  } else {
    configurePinmuxI2C("P9_21");
    configurePinmuxI2C("P9_22");
  }

  _i2c_fd = i2c_open(_i2c_bus, _addr);

  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

  if (_rows > 1) {
    _displayfunction |= LCD_2LINE;
  }

  // for some 1 line displays you can select a 10 pixel high font
  if ((_charsize != 0) && (_rows == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delay(50);

  // Now we pull both RS and R/W low to begin commands
  expanderWrite(_backlightval);  // reset expanderand turn backlight off (Bit 8 =1)
  delay(1000);

  //put the LCD into 4 bit mode
  // this is according to the hitachi HD44780 datasheet
  // figure 24, pg 46

  // we start in 8bit mode, try to set 4 bit mode
  write4bits(0x03 << 4);
  delayMicroseconds(4500); // wait min 4.1ms

  // second try
  write4bits(0x03 << 4);
  delayMicroseconds(4500); // wait min 4.1ms

  // third go!
  write4bits(0x03 << 4);
  delayMicroseconds(150);

  // finally, set to 4-bit interface
  write4bits(0x02 << 4);

  // set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for roman languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

  home();
}

/********** high level commands, for the user! */
void BBB_I2C_LCD::clear(){
  command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void BBB_I2C_LCD::home(){
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void BBB_I2C_LCD::setCursor(uint8_t col, uint8_t row){
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if (row > _rows) {
    row = _rows-1;    // we count rows starting w/0
  }
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void BBB_I2C_LCD::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void BBB_I2C_LCD::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void BBB_I2C_LCD::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void BBB_I2C_LCD::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void BBB_I2C_LCD::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void BBB_I2C_LCD::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void BBB_I2C_LCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void BBB_I2C_LCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/** moves cursor one character left */
void BBB_I2C_LCD::moveCursorLeft() {
  command(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
}

/** moves cursor one character left */
void BBB_I2C_LCD::moveCursorRight() {
  command(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
}


// This is for text that flows Left to Right
void BBB_I2C_LCD::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void BBB_I2C_LCD::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void BBB_I2C_LCD::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void BBB_I2C_LCD::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void BBB_I2C_LCD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    writeData(charmap[i]);
  }
}

// Turn the (optional) backlight off/on
void BBB_I2C_LCD::noBacklight(void) {
  _backlightval=LCD_NOBACKLIGHT;
  expanderWrite(0);
}

void BBB_I2C_LCD::backlight(void) {
  _backlightval=LCD_BACKLIGHT;
  expanderWrite(0);
}
bool BBB_I2C_LCD::getBacklight() {
  return _backlightval == LCD_BACKLIGHT;
}


/*********** mid level commands, for sending data/cmds */

inline void BBB_I2C_LCD::command(uint8_t value) {
  send(value, 0);
}

inline std::size_t BBB_I2C_LCD::writeData(uint8_t value) {
  send(value, Rs);
  return 1;
}


/************ low level data pushing commands **********/

// write either command or data
void BBB_I2C_LCD::send(uint8_t value, uint8_t mode) {
  uint8_t highnib=value&0xf0;
  uint8_t lownib=(value<<4)&0xf0;
  write4bits((highnib)|mode);
  write4bits((lownib)|mode);
}

void BBB_I2C_LCD::write4bits(uint8_t value) {
  expanderWrite(value);
  pulseEnable(value);
}

void BBB_I2C_LCD::expanderWrite(uint8_t _data){
  uint8_t val = _data | _backlightval;
  if (write(_i2c_fd, &val, 1) != 1)
    throw std::runtime_error(std::string("i2c_write_byte error: ") + strerror(errno));
}

void BBB_I2C_LCD::pulseEnable(uint8_t _data){
  expanderWrite(_data | En);  // En high
  delayMicroseconds(1);    // enable pulse must be >450ns

  expanderWrite(_data & ~En);  // En low
  delayMicroseconds(50);    // commands need > 37us to settle
}

//void BBB_I2C_LCD::load_custom_character(uint8_t char_num, uint8_t *rows){
  //createChar(char_num, rows);
//}

//void BBB_I2C_LCD::setBacklight(uint8_t new_val){
  //if (new_val) {
    //backlight();    // turn backlight on
  //} else {
    //noBacklight();    // turn backlight off
  //}
//}

void BBB_I2C_LCD::print(const char c[]){
  while(*c) {
    writeData(*c);
    c++;
  }
}

void BBB_I2C_LCD::print(char c){
  writeData(c);
}

void BBB_I2C_LCD::print(const int val, uint8_t base) {

  std::stringstream ss;

  switch (base) {
  case 16:
    ss << std::setw(2) << std::setfill('0') << std::hex << std::uppercase;
    break;
  case 8:
    ss <<std::setw(3) << std::setfill('0') << std::oct << std::uppercase;
    break;
  default:
    ss << std::dec;
  }

  ss << val;
  print(ss.str().c_str());
}


int BBB_I2C_LCD::i2c_open(uint8_t bus, uint8_t addr) {

  int file;
  std::string filename = "/dev/i2c-" + std::to_string(bus);

  if ((file = open(filename.c_str(),O_RDWR)) < 0)
  {
    throw std::runtime_error(std::string("i2c_open open error: ") + strerror(errno));
  }
  if (ioctl(file,I2C_SLAVE,addr) < 0)
  {
    throw std::runtime_error(std::string("i2c_open ioctl error: ") + strerror(errno));
  }
  return(file);
}

void BBB_I2C_LCD::delay(uint16_t msg) {
  struct timespec sleep;
  sleep.tv_sec = 0;
  sleep.tv_nsec = msg * 1000000;

  nanosleep(&sleep, NULL);
}


void BBB_I2C_LCD::delayMicroseconds(uint16_t usg) {
  struct timespec sleep;
  sleep.tv_sec = 0;
  sleep.tv_nsec = usg * 1000;

  nanosleep(&sleep, NULL);
}


