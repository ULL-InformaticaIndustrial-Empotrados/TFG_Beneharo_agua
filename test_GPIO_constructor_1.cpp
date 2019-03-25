#include "BBB_GPIO_pin.hpp"

#include <iostream>

int main() {

  BBB_GPIO_pin p1("P9_23");

  std::cout << "p1 es pin '" << p1.getPosition() << "' con número "
      << (int)p1.getNumber() << std::endl;

  BBB_GPIO_pin p2("P8_04");

  std::cout << "p2 es pin '" << p2.getPosition() << "' con número "
      << (int)p2.getNumber() << std::endl;

  BBB_GPIO_pin p3("P8_4");

  std::cout << "p3 es pin '" << p3.getPosition() << "' con número "
      << (int)p3.getNumber() << std::endl;

}
