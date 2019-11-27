
#include "Valvula.hpp"
#include <iostream>
#include <chrono>
#include <thread>

//Constructor con iniciliazador de los pines utilizados
Valvula::Valvula() : _v1("P9_41"), _v2("P9_25"){

  _v1.setDirection("out");
  _v1.setValue(0);

  _v2.setDirection("out");
  _v2.setValue(0);

  std::cout << "Indica milisegundos: ";
  std::cin >> numMillis;
}



void Valvula::abreValvula(){

    std::cout << "Pulsa 'a' para abrir." << std::endl;

    char c;
    std::cin >> c;


    if(c == 'a'){
      _v1.setValue(0);
      std::this_thread::sleep_for(std::chrono::milliseconds(numMillis));
      _v1.setValue(1);
    }
}



void Valvula::cierraValvula(){

    std::cout << "Pulsa 'c' para cerrar." << std::endl;

    char c;
    std::cin >> c;


    if(c == 'c'){
      _v2.setValue(0);
      std::this_thread::sleep_for(std::chrono::milliseconds(numMillis));
      _v2.setValue(1);
    }
}



