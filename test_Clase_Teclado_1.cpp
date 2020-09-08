// int main() de la clase 'Teclado'

#include "Teclado.hpp"
#include <iostream>

#define MS_OBTENER_TECLA 15000


/*
    Para usar otros pines
    int main("especificar los pines nuevos aqui")
*/




int main() {

  Teclado tec; //Objeto-atributo de la clase 'Teclado'

  while(1) {

    char key = tec.obtenerTecla(MS_OBTENER_TECLA);

    std::cout << "Se pulsó la tecla " << key << std::endl;
  }

  return 0;
}


