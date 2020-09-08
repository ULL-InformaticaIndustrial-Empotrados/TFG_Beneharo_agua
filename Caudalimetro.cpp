
#include "Caudalimetro.hpp"
#include <iostream>
#include <thread>

//El numero de pin que se le pase como parámetro al constructor
//El constructor reciba un string
//A la clase Caudalimetro el string("P8_36") se le pase como parámetro al constructor.
//Esto se hace por si en el futuro se cambiase de pin, no tener que estar cambiando el código


Caudalimetro::Caudalimetro(){
/* Variables locales que dejan de tener influencia fuera del constructor

  BBB_GPIO_pin p1("P9_12"); //Creamos objeto 'p1'
  p1.setDirection("in"); //Configuramos como entrada

  _intrr.addPin(p1); //Añade el pin
  p1.setEdge("rising"); //Flanco de subida
*/


  //Crea un nuevo objeto de tipo thread con esas caracteristicas y
  //devuelveme su direccion. Esa direccion de memoria la voy a guardar en '_pthilo1'
  _pthilo1 = new std::thread(&Caudalimetro::cuentaPulsos, this);


}



unsigned Caudalimetro::getNumeroPulsos(){

  return _nPulsos;
}


void Caudalimetro::cuentaPulsos(){

  //Variables locales
  BBB_GPIO_pin p1("P9_12"); //Creamos objeto 'p1'
  p1.setDirection("in"); //Configuramos como entrada
  _intrr.addPin(p1); //Añade el pin
  p1.setEdge("rising"); //Flanco de subida


  //std::cerr << "Entramos en cuenta pulso" << std::endl;
  while(1){
    //std::cerr << "Esperamos un pulso" << std::endl;
    _intrr.edgeWait(); //Espera hasta que haya un pulso, desde que se produzca un flanco de subida
    //std::cerr << "Recibido pulso" << std::endl;
    _nPulsos++; //Va sumando los flancos de subida
  }

}

float Caudalimetro::getLitros(){
  return getNumeroPulsos() / pulsosPorLitro; //Devuelve el número de litros
}


void Caudalimetro::resetNumeroPulsos(){

  _nPulsos = 0;
}


