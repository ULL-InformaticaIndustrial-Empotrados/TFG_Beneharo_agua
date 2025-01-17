#ifndef _BBB_I2C_LCD_
#define _BBB_I2C_LCD_

#include <cstdint>
#include <string>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

/**
 * This is the driver for the Liquid Crystal LCD displays that use the I2C bus.
 *
 * After creating an instance of this class, first call begin() before anything else.
 * The backlight is on by default, since that is the most likely operating mode in
 * most cases.
 */
class BBB_I2C_LCD {
public:
  /**
   * Constructor
   *
   * @param i2c_bus  BBB i2c bus number used for LCD:
   *          1 (for i2c-1 on pins P9_17 & P9_18) or 2 (for i2c-2 on pins P9_21 & P9_22)
   * @param lcd_addr  I2C slave address of the LCD display. Most likely printed on the
   *          LCD circuit board, or look in the supplied LCD documentation.
   * @param lcd_cols  Number of columns your LCD display has.
   * @param lcd_rows  Number of rows your LCD display has.
   * @param charsize  The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
   */
  BBB_I2C_LCD(uint8_t i2c_bus, uint8_t lcd_addr, uint8_t lcd_cols,
      uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS);

  /**
   * Set the LCD display in the correct begin state, must be called before
   *  anything else is done.
   */
  void begin();

   /**
    * Remove all the characters currently shown. Next print/write operation will start
    * from the first position on LCD display.
    */
  void clear();

  /**
   * Next print/write operation will will start from the first position on the LCD display.
   */
  void home();

   /**
    * Do not show any characters on the LCD display. Backlight state will remain unchanged.
    * Also all characters written on the display will return, when the display in enabled again.
    */
  void noDisplay();

  /**
   * Show the characters on the LCD display, this is the normal behaviour.
   * This method should only be used after noDisplay() has been used.
   */
  void display();

  /**
   * Do not blink the cursor indicator.
   */
  void noBlink();

  /**
   * Start blinking the cursor indicator.
   */
  void blink();

  /**
   * Do not show a cursor indicator.
   */
  void noCursor();

  /**
   * Show a cursor indicator, cursor can blink on not blink. Use the
   * methods blink() and noBlink() for changing cursor blink.
   */
  void cursor();

  /** Scroll the display left without changing the RAM */
  void scrollDisplayLeft();

  /** Scroll the display right without changing the RAM */
  void scrollDisplayRight();

  /** moves cursor one character left */
  void moveCursorLeft();

  /** moves cursor one character left */
  void moveCursorRight();

  /** Set Left to Right mode. Next character in the right of current one*/
  void leftToRight();

  /** Set Right to Left mode. Next character in the left of current one*/
  void rightToLeft();

  void shiftIncrement();
  void shiftDecrement();

  /** Turn backlight off */
  void noBacklight();

  /** Turn backlight off */
  void backlight();

  /** Gets backlight state (ture → on, false → off) */
  bool getBacklight();

  /** set shift mode. Next character shift displays, cursor not moved */
  void autoscroll();
  /** Disable shift mode. Next character move the cursor */
  void noAutoscroll();

  /** Allows us to fill the first 8 CGRAM locations with custom characters */
  void createChar(uint8_t, uint8_t[]);

  /**
   * Change de cursor position.
   *
   * @param col  new cursor column
   * @param row  new cursor row
   */
  void setCursor(uint8_t col, uint8_t row);

  /** Sends data to the display*/
  virtual std::size_t writeData(uint8_t);

  /** Sends command to the display */
  void command(uint8_t);

  /** Same of `blink()` */
  inline void blink_on() { blink(); }
  /** Same of `noBlink()` */
  inline void blink_off() { noBlink(); }
  /** Same of `cursor()` */
  inline void cursor_on() { cursor(); }
  /** Same of `noCursor()` */
  inline void cursor_off() { noCursor(); }

//// Compatibility API function aliases
  //void setBacklight(uint8_t new_val);        // alias for backlight() and nobacklight()
  //void load_custom_character(uint8_t char_num, uint8_t *rows);  // alias for createChar()

  /**
   * Send characters to display
   */
  void print(const char[]);

  /**
   * Send one characters to display
   */
  void print(char);


  /**
   * Print int to display.
   *
   * @param val  int value to show
   * @param base numeric base, default 10
   */
  void print(const int val, uint8_t base=10);

private:
  void configurePinmuxI2C(std::string pinName);

  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void expanderWrite(uint8_t);
  void pulseEnable(uint8_t);

  int i2c_open(uint8_t bus, uint8_t addr);
  void delay(uint16_t msg);
  void delayMicroseconds(uint16_t usg);

  uint8_t _i2c_bus;
  uint8_t _addr;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _charsize;
  uint8_t _backlightval;

  int _i2c_fd = 0;  // i2c file descriptor
};

#endif // _BBB_I2C_LCD_
