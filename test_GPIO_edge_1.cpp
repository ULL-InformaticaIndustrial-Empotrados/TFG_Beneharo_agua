#include "BBB_GPIO_pin.hpp"
#include "BBB_GPIO_Interrupts.hpp"

#include <iostream>
#include <chrono>
#include <thread>

int main() {

  BBB_GPIO_pin p1("P8_45");

  std::cout << "p1 es pin '" << p1.getPosition() << "' con número "
      << p1.getNumber() << std::endl;

  p1.setDirection("in");
  std::cout <<  "La dirección es entrada?  '" << p1.isDirectionIn()
      << "'" << std::endl;

  p1.setPullMode("gpio_pd");
  std::cout <<  "Modo de pull_up  '" << p1.getPullMode() << "'" << std::endl;

  p1.setActiveLow(false);
  std::cout <<  "Modo de activeLow  '" << p1.isActiveLow() << "'" << std::endl;


  //Agregar 1 interrupcion
  p1.setEdge("rising");

  BBB_GPIO_Interrupts intrr;

  intrr.addPin(p1);

  for (int i = 0; i < 10; i++) {
    std::cout << "\nEsperamos algún cambio" << std::endl;
    intrr.edgeWait();
    std::cout << "El valor del pin " << p1.getPosition() << " es "
        << p1.getValue() << std::endl;
  }

}


