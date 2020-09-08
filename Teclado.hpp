//Declaración de la clase 'Teclado'

#ifndef _TECLADO_
#define _TECLADO_

#include "BBB_GPIO_pin.hpp"
#include "BBB_GPIO_Interrupts.hpp"



class Teclado {

	protected: //Es común a todas las clases

    BBB_GPIO_pin _c1;
    BBB_GPIO_pin _c2;
    BBB_GPIO_pin _c3;

    BBB_GPIO_pin _f1;
    BBB_GPIO_pin _f2;
    BBB_GPIO_pin _f3;
    BBB_GPIO_pin _f4;



	public:

		Teclado(); //Constructor por defecto de la clase

    //Constructor alternativo para usar pines diferentes del GPIO
    // Teclado(std::string monbreC1, std::string monbreC2);

    //char obtenerTecla(int msTimeout = -1); //Por defecto, tiempo infinito
    char obtenerTecla(int msTimeout = -1);



};


#endif



