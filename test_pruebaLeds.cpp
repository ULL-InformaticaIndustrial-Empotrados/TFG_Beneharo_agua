//Prueba de leds

#include "BBB_GPIO_pin.hpp"

#include <iostream>
#include <chrono>
#include <thread>

int main() {
	BBB_GPIO_pin p1("P9_41");
	
	p1.setDirection("out");
	
	p1.setValue(1);
	
	std::this_thread::sleep_for(std::chrono::seconds(3));
	
	p1.setValue(0);
  

}


