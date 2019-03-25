#include "BBB_I2C_LCD.hpp"
#include <chrono>
#include <thread>

#define HEX (16)

// display all keycodes
void displayKeyCodes(BBB_I2C_LCD& lcd) {

  auto pausa = std::chrono::seconds(5);

  uint8_t i = 0;

  while (1) {
    lcd.clear();
    lcd.print("Codes 0x");
    lcd.print(i, HEX);
    lcd.print("-0x");
    lcd.print(i + 16, HEX);
    lcd.setCursor(0, 1);

    for (int j = 0; j < 16; j++) {
      lcd.writeData(i + j);
    }
    i += 16;

    std::this_thread::sleep_for(pausa);
  }
}


int main() {

  // Set the LCD address to 0x3f for a 20 chars and 2 line display in i2c-1
  BBB_I2C_LCD lcd(1, 0x3f, 20, 4);

  uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
  uint8_t note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
  uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
  uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
  uint8_t duck[8]  = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
  uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
  uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
  uint8_t retarrow[8] = {  0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

  lcd.begin();
  lcd.backlight();

  lcd.createChar(0, bell);
  lcd.createChar(1, note);
  lcd.createChar(2, clock);
  lcd.createChar(3, heart);
  lcd.createChar(4, duck);
  lcd.createChar(5, check);
  lcd.createChar(6, cross);
  lcd.createChar(7, retarrow);
  lcd.home();

  lcd.print("Hello world...");
  lcd.setCursor(0, 1);
  lcd.print(" I ");
  lcd.writeData(3);
  lcd.print(" BBB !");
  std::this_thread::sleep_for(std::chrono::seconds(5));

  displayKeyCodes(lcd);
}

