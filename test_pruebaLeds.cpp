//Prueba de leds

#include "BBB_GPIO_pin.hpp" //Declaración de la clase

#include <iostream>
#include <chrono>
#include <thread>

int main() {
	
	BBB_GPIO_pin p1("P9_41"); //Creamos un objeto 'p1' de la clase 'BBB_GPIO_pin'
	
	p1.setDirection("out"); //Lo configuramos como salida
	
	p1.setValue(1); //Con valor '1' , es decir, con tensión (3,3V)
	
	std::this_thread::sleep_for(std::chrono::seconds(1)); //Parpadea durante 1 segundo
	
	p1.setValue(0); //Después del tiempo transcurrido, poner a 0, es decir, apagar
	
	
  

}


