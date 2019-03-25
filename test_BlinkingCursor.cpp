#include "BBB_I2C_LCD.hpp"
#include <chrono>
#include <thread>

int main() {

  auto periodo = std::chrono::seconds(4);
  
  // Set the LCD address to 0x3f for a 20 chars and 2 line display in i2c-1
  BBB_I2C_LCD lcd(1, 0x3f, 20, 4);

  // initialize the LCD
  lcd.begin();

  bool blinking = true;
  lcd.cursor();

  while (1) {
    if (blinking) {
      lcd.clear();
      lcd.print("No cursor blink");
      lcd.noBlink();
      blinking = false;
    } else {
      lcd.clear();
      lcd.print("Cursor blink");
      lcd.blink();
      blinking = true;
    }
    std::this_thread::sleep_for(periodo);
  }
}
