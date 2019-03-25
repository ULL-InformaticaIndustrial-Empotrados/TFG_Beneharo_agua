#include "BBB_I2C_LCD.hpp"


int main() {
  // Set the LCD address to 0x3f for a 20 chars and 2 line display in i2c-1
  BBB_I2C_LCD lcd(1, 0x3f, 20, 4);

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");

}
