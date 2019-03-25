#include "BBB_GPIO_pin.hpp"

#include <iostream>
#include <chrono>
#include <thread>

int main() {

  BBB_GPIO_pin p1("P9_23");

  std::cout << "p1 es pin '" << p1.getPosition() << "' con número "
      << (int)p1.getNumber() << std::endl;

  p1.setDirection("out");
  p1.setValue(1);

  BBB_GPIO_pin p2("P9_41");

  std::cout << "p2 es pin '" << p2.getPosition() << "' con número "
      << (int)p2.getNumber() << std::endl;

  p2.setDirection("out");
  p2.setValue(1);

  bool va = false;
  for (int i = 0; i < 10; i++) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p1.setValue(va);
    p2.setValue(!va);
    std::cout << "p1: '" << p1.getPosition() << "' con valor "
        << p1.getValue() << std::endl;
    std::cout << "p2: '" << p2.getPosition() << "' con valor "
        << p2.getValue() << std::endl;
    va = !va;
  }

  std::cout <<  "La dirección es entrada?  '" << p1.isDirectionIn()
      << "'" << std::endl;
  p1.setDirection("in");
  std::cout <<  "La dirección es entrada?  '" << p1.isDirectionIn()
      << "'" << std::endl;

}
