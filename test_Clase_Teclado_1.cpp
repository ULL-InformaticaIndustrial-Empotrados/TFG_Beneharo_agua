//Código que contiene el main(), comienza por test_

#include "Teclado.hpp"
#include <iostream>

//Para usar otros pines
//int main("especificar los pines nuevos aqui")

int main() {

  Teclado tec; //Objeto-atributo de la clase 'Teclado'

  while(1) {

    char key = tec.obtenerTecla(5000);

    std::cout << "Se pulsó la tecla " << key << std::endl;
  }
}


