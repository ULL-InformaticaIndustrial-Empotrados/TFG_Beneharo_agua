
#include "Caudalimetro.hpp"
#include <iostream>
#include <thread>



Caudalimetro::Caudalimetro(){
/*
  BBB_GPIO_pin p1("P9_12"); //Creamos objeto 'p1'
  p1.setDirection("in"); //Configuramos como entrada

  _intrr.addPin(p1); //Añade el pin
  p1.setEdge("rising"); //Flanco de subida
*/
  _pthilo1 = new std::thread(&Caudalimetro::cuentaPulsos, this); //Lee pulsos


}



unsigned Caudalimetro::getNumeroPulsos(){

  return _nPulsos;
}


void Caudalimetro::cuentaPulsos(){

  //Ponemos ésto aqui ya que se va a usar en concreto aqui como variables locales
  BBB_GPIO_pin p1("P9_12"); //Creamos objeto 'p1'
  p1.setDirection("in"); //Configuramos como entrada

  _intrr.addPin(p1); //Añade el pin
  p1.setEdge("rising"); //Flanco de subida

  //Bucle infinito
  //std::cerr << "Entramos en cuenta pulso" << std::endl;
  while(1){
    //std::cerr << "Esperamos un pulso" << std::endl;
    _intrr.edgeWait(); //Espera hasta que haya un pulso, desde que se produzca un flanco de subida
    //std::cerr << "Recibido pulso" << std::endl;
    _nPulsos++;
  }

}




void Caudalimetro::resetNumeroPulsos(){

  _nPulsos = 0;
}


