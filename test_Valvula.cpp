
#include "Valvula.hpp"
#include <iostream>
#include <thread>



int main(){

  Valvula valv;


  while(1){

    std::cout << "Pulsa 'a' para abrir y 'c' para cerrar" << std::endl;

    char c;
    std::cin >> c;


    if(c == 'a'){
      valv.abreValvula();
    }


    if(c == 'c'){
      valv.cierraValvula();
    }


  }


  return 0;
}



