//Función de teclado -- Falla la compilación ya que hay que declarar los pines como globales
//Sin ebargo, no es aconsejable usarlos globales, ya que suelen dar problemas,
//por eso se usan las clases.

#include "BBB_GPIO_pin.hpp"

#include <iostream>
#include <chrono>
#include <thread>



void inicializaTeclado() {
    //Ésto se ejecuta solo la primera vez
    //Cuando hagamos la clase teclado, ésto es lo que lleva el constructor, es decir, los pines

    //Las 3 columnas configuradas como salidas y forzadas a '1'
    BBB_GPIO_pin c1("P8_34");
    c1.setDirection("out");
    c1.setValue(1);

    BBB_GPIO_pin c2("P8_36");
    c2.setDirection("out");
    c2.setValue(1);

    BBB_GPIO_pin c3("P8_38");
    c3.setDirection("out");
    c3.setValue(1);



    //Las 4 filas configuradas como entradas con 'pull-down' ('0' cuando está al aire)
    BBB_GPIO_pin f1("P8_40");
    f1.setDirection("in"); //Entrada
    f1.setPullMode("gpio_pd"); //Modo usado
    f1.setActiveLow(false); //Activar la lógica desordenada del pulsador pull_down para poner a '0'

    BBB_GPIO_pin f2("P8_42");
    f2.setDirection("in");
    f2.setPullMode("gpio_pd");
    f2.setActiveLow(false);

    BBB_GPIO_pin f3("P8_44");
    f3.setDirection("in");
    f3.setPullMode("gpio_pd");
    f3.setActiveLow(false);

    BBB_GPIO_pin f4("P8_46");
    f4.setDirection("in");
    f4.setPullMode("gpio_pd");
    f4.setActiveLow(false);
}


char obtenerTecla(){

  char teclaPulsada = ' ';

    //Aseguramos que las columnas estén a '1' siempre antes del bucle, para evitar problemas
    c1.setValue(1);
    c2.setValue(1);
    c3.setValue(1);

    //Finaliza cuando todas las filas estén a '0', es decir, cuando todas las teclas estén sin pulsar
    while( (f1.getValue() == 0) && (f2.getValue() == 0) && (f3.getValue() == 0)
        && (f4.getValue() == 0) ){

          std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(20)); //Esperamos 20 mseg para evitar los rebotes de los pulsadores

    //Columna 1
    c1.setValue(1); //Poner 1ª columna a '1' y las demás a '0'
    c2.setValue(0);
    c3.setValue(0);

    if(f1.getValue() == 1 ){
      std::cerr << "Se ha pulsado el '1'" << std::endl;
      teclaPulsada = '1';

    }
    if(f2.getValue() == 1 ){
      std::cout << "Se ha pulsado el '4'" << std::endl;
      teclaPulsada = '4';
    }
    if(f3.getValue() == 1 ){
      std::cout << "Se ha pulsado el '7'" << std::endl;
    }
    if(f4.getValue() == 1 ){
      std::cout << "Se ha pulsado el '*'" << std::endl;
    }


    //Columna 2
    c1.setValue(0);
    c2.setValue(1); //Poner 2ª columna a '1' y las demás a '0'
    c3.setValue(0);

    if(f1.getValue() == 1 ){
      std::cout << "Se ha pulsado el '2'" << std::endl;
    }
    if(f2.getValue() == 1 ){
      std::cout << "Se ha pulsado el '5'" << std::endl;
    }
    if(f3.getValue() == 1 ){
      std::cout << "Se ha pulsado el '8'" << std::endl;
    }
    if(f4.getValue() == 1 ){
      std::cout << "Se ha pulsado el '0'" << std::endl;
    }


    //Columna 3
    c1.setValue(0);
    c2.setValue(0);
    c3.setValue(1); //Poner 3ª columna a '1' y las demás a '0'

    if(f1.getValue() == 1 ){
      std::cout << "Se ha pulsado el '3'" << std::endl;
    }
    if(f2.getValue() == 1 ){
      std::cout << "Se ha pulsado el '6'" << std::endl;
    }
    if(f3.getValue() == 1 ){
      std::cout << "Se ha pulsado el '9'" << std::endl;
    }
    if(f4.getValue() == 1 ){
      std::cout << "Se ha pulsado el '#'" << std::endl;
    }



    //Bucle para salir de 'tecla pulsada'

    //Aseguramos que las columnas estén a '1' siempre antes del bucle, para evitar problemas
    c1.setValue(1);
    c2.setValue(1);
    c3.setValue(1);

    //Finaliza cuando alguna fila esté a '1', es decir, alguna tecla se hubiese pulsado
    while( (f1.getValue() == 1) || (f2.getValue() == 1) || (f3.getValue() == 1)
        || (f4.getValue() == 1) ){

            std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

   return teclaPulsada;
}





int main() {

  inicializaTeclado();

  std::cout << "Ver tecla 1" << std::endl;

  while(1) {

    char key = obtenerTecla();

    std::cout << "Se pulsó la tecla " << key << std::endl;
  }

  std::cout << "Ver tecla 1" << std::endl;
}


//Ahora hay que hacer una función que lea teclas, junto con el main

//Después hay que crear una clase con .hpp y .cpp

//Se busca hacer lo mismo que hasta ahora pero haciendo uso de std::string en vez de std::cout








