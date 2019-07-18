#include "BBB_GPIO_pin.hpp" //Incluye la clase
#include "BBB_GPIO_Interrupts.hpp"
#include <iostream>
#include <thread>



//Función que actualiza nPulsos
void numPulsos(int& nPulsos){

  BBB_GPIO_pin p1("P9_12"); //Creamos objeto 'p1'
  p1.setDirection("in"); //Configuramos como entrada
  BBB_GPIO_Interrupts intrr; //Objeto de la clase 'BBB_GPIO_Interrupts'
  intrr.addPin(p1); //Añade el pin
  p1.setEdge("rising"); //Flanco de subida

  //Bucle infinito
  while(1){
    intrr.edgeWait(); //Espera hasta que haya un pulso, desde que se produzca un flanco de subida
    nPulsos++;

    //std::cout << "Detectado pulso: " << nPulsos << std::endl;
  }


}


int main(){

  int nPulsos = 0;

  //Llamada a la función
  //numPulsos(nPulsos);


  //Hilo solo pendiente de los pulsos, por lo que no se pierde ninguno
  //Se ejecuta el hilo1 en un segundo plano (multitarea)
  std::thread hilo1(numPulsos, std::ref(nPulsos)); //Lee nPulsos

  while(1){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::cout << "Tras 0.1 segundos, " << nPulsos << " pulsos" << std::endl;

  }


  return 0;
}


