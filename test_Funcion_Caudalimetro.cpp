
#include "Caudalimetro.hpp"

#include <iostream>
#include <thread>


int main(){

  std::cerr << "Comienzo" << std::endl;


  Caudalimetro caud;

  std::cerr << "Bucle" << std::endl;

  while(1){
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "Tras 5 segundos, " << caud.getNumeroPulsos() << " pulsos"
      << std::endl;
  }



  return 0;
}

