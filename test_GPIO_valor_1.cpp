#include "BBB_GPIO_pin.hpp"
//Primer dia -- Programa prueba del gpio
#include <iostream>
#include <chrono>
#include <thread>

int main() {

  BBB_GPIO_pin p1("P9_27");

  std::cout << "p1 es pin '" << p1.getPosition() << "' con número "
      << (int)p1.getNumber() << std::endl;

  p1.setDirection("in");
  std::cout <<  "La dirección es entrada?  '" << p1.isDirectionIn()
      << "'" << std::endl;

  p1.setPullMode("gpio_pu");
  std::cout <<  "Modo de pull_up  '" << p1.getPullMode() << "'" << std::endl;

  p1.setActiveLow(true);
  std::cout <<  "Modo de activeLow  '" << p1.isActiveLow() << "'" << std::endl;

  BBB_GPIO_pin p2("P8_45");

  std::cout << "p2 es pin '" << p2.getPosition() << "' con número "
      << (int)p2.getNumber() << std::endl;

  p2.setDirection("in");
  std::cout <<  "La dirección es entrada?  '" << p2.isDirectionIn()
      << "'" << std::endl;

  p2.setPullMode("gpio_pd");
  std::cout <<  "Modo de pull_up  '" << p2.getPullMode() << "'" << std::endl;

  p2.setActiveLow(false);
  std::cout <<  "Modo de activeLow  '" << p2.isActiveLow() << "'" << std::endl;

  for (int i = 0; i < 10; i++) {
    std::cout << "El valor del pin " << p1.getPosition() << " es "
        << p1.getValue() << std::endl;
    std::cout << "El valor del pin " << p2.getPosition() << " es "
        << p2.getValue() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  p1.setDirection("out");
  std::cout <<  "La dirección es entrada?  '" << p1.isDirectionIn()
      << "'" << std::endl;

  p1.setPullMode("gpio_pd");
  std::cout <<  "Modo de pull_up  '" << p1.getPullMode() << "'" << std::endl;

  p1.setActiveLow(false);
  std::cout <<  "Modo de activeLow  '" << p1.isActiveLow() << "'" << std::endl;

  p2.setDirection("out");
  std::cout <<  "La dirección es entrada?  '" << p2.isDirectionIn()
      << "'" << std::endl;

  p2.setPullMode("gpio_pu");
  std::cout <<  "Modo de pull_up  '" << p2.getPullMode() << "'" << std::endl;

  p2.setActiveLow(true);
  std::cout <<  "Modo de activeLow  '" << p2.isActiveLow() << "'" << std::endl;

}
