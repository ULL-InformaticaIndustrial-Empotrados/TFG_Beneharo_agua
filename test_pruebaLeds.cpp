//Prueba de parpadeo de 2 leds

#include "BBB_GPIO_pin.hpp" //Incluye la clase

#include <iostream>
#include <chrono>
#include <thread>

int main() {
	
	BBB_GPIO_pin p1("P9_41"); //Creamos un objeto 'p1' de la clase 'BBB_GPIO_pin'
	BBB_GPIO_pin p2("P9_25");
	
	for (int i = 0; i < 10; i++) {
		p1.setDirection("out"); //Lo configuramos como salida
		p2.setDirection("out");
		p1.setValue(1); //Con valor '1' , es decir, con tensión (3,3V)
		p2.setValue(1);
		std::this_thread::sleep_for(std::chrono::seconds(1)); //Parpadea durante 1 segundo
		
		p1.setValue(0); //Después del tiempo transcurrido, poner a 0, es decir, apagar
		p2.setValue(0);
		std::this_thread::sleep_for(std::chrono::seconds(1)); //Parpadea durante 1 segundo
	}
}



