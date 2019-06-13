#include "BBB_I2C_LCD.hpp" //Incluye la clase 'BBB_I2C_LCD'


int main() {
  // Set the LCD address to 0x3f for a 20 chars and 2 line display in i2c-1
  BBB_I2C_LCD lcd(1, 0x3f, 20, 4); //Se crea objeto lcd

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight(); //Enciende la luz del lcd
  lcd.print("Hello, world 1!"); //Imprime

  lcd.print('+');



}

