
#include "Valvula.hpp"
#include <iostream>
#include <thread>



int main(){

  Valvula valv;


  while(1){

    valv.abreValvula();

    valv.cierraValvula();
  }


  return 0;
}



