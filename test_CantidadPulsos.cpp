/*
  Función para contabilizar el numero de pulsos por cada litro de agua
*/

#include "BBB_GPIO_pin.hpp" //Incluye la clase
#include "BBB_GPIO_Interrupts.hpp"
#include <iostream>
#include <thread>

/*
 * Luego hay que realizar una función que contabilice el numero de pulsos,
 * por cada litro de agua. Se puede hacer la prueba, cogiendo un
 * recipiente de 1 litro, llenarlo y contabilizar los pulsos empleados.
 * Para ello hay que usar una variable int que vaya incrementandose por cada
 * pulso detectado y meter el bucle while en una función aparte que vaya
 * incrementado cada vez que detecte un pulso.
*/


/*
//Configuramos solo 1 hilo
void fHilo1(int v1, double v2){

}


*/

int main(){

  BBB_GPIO_pin p1("P9_12");
  p1.setDirection("in"); //Configuramos como entrada

  //std::thread hilo1(fHilo1);


  BBB_GPIO_Interrupts intrr; //Objeto de la clase 'BBB_GPIO_Interrupts'

  intrr.addPin(p1); //Añade el pin

  p1.setEdge("rising"); //Flanco de subida

  //Bucle infinito
  while(1){
    intrr.edgeWait(); //Espera hasta que haya un pulso, desde que se produzca un flanco de subida
    std::cout << "detectado pulso" << std::endl;

  }


  return 0;
}


