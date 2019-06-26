#ifndef _TECLADO_
#define _TECLADO_

#include <cstdint> //int8_t, ...
#include <string>
#include <vector>
#include "BBB_GPIO_pin.hpp" //Incluye la clase
#include "BBB_GPIO_Interrupts.hpp"
#include <string>


//Ahora hay que hacer una función que lea teclas, junto con el main

//Después hay que crear una clase con .hpp y .cpp

//Se busca hacer lo mismo que hasta ahora pero haciendo uso de std::string en vez de std::cout


class Teclado {

	protected: //Atributos

    BBB_GPIO_pin _c1;
    BBB_GPIO_pin _c2;
    BBB_GPIO_pin _c3;

    BBB_GPIO_pin _f1;
    BBB_GPIO_pin _f2;
    BBB_GPIO_pin _f3;
    BBB_GPIO_pin _f4;



	public: //Métodos

		Teclado(); //Constructor por defecto

    //Constructor alternativo para usar pines diferentes del GPIO
    // Teclado(std::string monbreC1, std::string monbreC2);

    char obtenerTecla(int msTimeout = -1); //Por defecto, tiempo infinito



};


#endif



