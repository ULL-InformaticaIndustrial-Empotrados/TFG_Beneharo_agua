
#include "Teclado.hpp"
#include <iostream>

int main() {

  Teclado tec; //Objeto de la clase 'Teclado'

  while(1) {

    char key = tec.obtenerTecla();

    std::cout << "Se pulsó la tecla " << key << std::endl;
  }
}


