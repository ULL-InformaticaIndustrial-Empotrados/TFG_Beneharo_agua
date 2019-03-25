/**
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 */
#include "BBB_I2C_LCD.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {

  // Set the LCD address to 0x3f for a 20 chars and 2 line display in i2c-1
  BBB_I2C_LCD lcd(1, 0x3f, 20, 4);

  // initialize the LCD
  lcd.begin();

  lcd.backlight();

  std::cout << "Introduce caracteres a envíar: " << std::endl;

  bool blink = false;
  bool off = false;
  bool cursor = false;
  char c;
  std::cin.get(c);
  while (std::cin.good()) {
    switch (c) {
    case '\n':
      break; // ingnore CR
    case '\x02': // ^B
      lcd.clear();
      break;
    case '\x08': // ^H
      lcd.home();
      break;
    case '\x10': // ^P
      blink = !blink;
      if (blink) lcd.blink(); else lcd.noBlink();
      break;
    case '\x0F': // ^O
      off = !off;
      if (off) lcd.noDisplay(); else lcd.display();
      break;
    case '\x14': // ^T
      cursor = !cursor;
      if (cursor) lcd.cursor(); else lcd.noCursor();
      break;
    case '\x0B': // ^K cursor right
      lcd.command((1 << 4));
      break;
    case '\x0C': // ^L cursor left
      lcd.command((1 << 4)|(1 << 2));
      break;
    case '\x06': // ^F cursor right
      lcd.command((1 << 4)|(1 << 3));
      break;
    case '\x07': // ^G cursor left
      lcd.command((1 << 4)|(1 << 3)|(1 << 2));
      break;



    default:
      lcd.writeData(c);
    }
    std::cin.get(c);
  }
}
