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

  p1.setEdge("rising");

  BBB_GPIO_pin p2("P9_27");

  std::cout << "p2 es pin '" << p2.getPosition() << "' con número "
      << p2.getNumber() << std::endl;

  p2.setDirection("in");
  std::cout <<  "La dirección es entrada?  '" << p2.isDirectionIn()
      << "'" << std::endl;

  p2.setPullMode("gpio_pu");
  std::cout <<  "Modo de pull_up  '" << p2.getPullMode() << "'" << std::endl;

  p2.setActiveLow(true);
  std::cout <<  "Modo de activeLow  '" << p2.isActiveLow() << "'" << std::endl;

  p2.setEdge("rising");

  BBB_GPIO_Interrupts intrr;

  intrr.addPin(p1);
  intrr.addPin(p2);
  int timeOut = 1000;

  for (int i = 0; i < 5; i++) {
    std::cout << "\nEsperamos algún cambio " << timeOut << std::endl;
    BBB_GPIO_pin* pm = intrr.edgeWait(timeOut);
    if (pm != nullptr)
      std::cout << "Pin modificado: " << pm-> getPosition() << std::endl;
    else
      std::cout << "TimeOut" << std::endl;
    std::cout << "El valor del pin " << p1.getPosition() << " es "
        << p1.getValue() << std::endl;
    std::cout << "El valor del pin " << p2.getPosition() << " es "
        << p2.getValue() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  std::cout << "\n\nSacamos el pin " << p1.getPosition() << std::endl;
  intrr.delPin(p1);
  timeOut = 2000;

  for (int i = 0; i < 5; i++) {
    std::cout << "\nEsperamos algún cambio " << timeOut << std::endl;
    BBB_GPIO_pin* pm = intrr.edgeWait(timeOut);
    if (pm != nullptr)
      std::cout << "Pin modificado: " << pm-> getPosition() << std::endl;
    else
      std::cout << "TimeOut" << std::endl;
    std::cout << "El valor del pin " << p1.getPosition() << " es "
        << p1.getValue() << std::endl;
    std::cout << "El valor del pin " << p2.getPosition() << " es "
        << p2.getValue() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  std::cout << "\n\nIntroducimos el pin " << p1.getPosition() << std::endl;
  intrr.addPin(p1);
  timeOut = 500;

  for (int i = 0; i < 5; i++) {
    std::cout << "\nEsperamos algún cambio " << timeOut << std::endl;
    BBB_GPIO_pin* pm = intrr.edgeWait(timeOut);
    if (pm != nullptr)
      std::cout << "Pin modificado: " << pm-> getPosition() << std::endl;
    else
      std::cout << "TimeOut" << std::endl;
    std::cout << "El valor del pin " << p1.getPosition() << " es "
        << p1.getValue() << std::endl;
    std::cout << "El valor del pin " << p2.getPosition() << " es "
        << p2.getValue() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

}
