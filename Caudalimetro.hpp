
#ifndef _CAUDALIMETRO_
#define _CAUDALIMETRO_

#include "BBB_GPIO_pin.hpp"
#include "BBB_GPIO_Interrupts.hpp"
#include <thread>


//A la clase Caudalimetro el string(P8_36) se le pase como parámetro al constructor.


class Caudalimetro {


  protected:

    unsigned _nPulsos = 0; //Inicializada a 0
    BBB_GPIO_Interrupts _intrr;//Objeto de la clase 'BBB_GPIO_Interrupts'
    void cuentaPulsos();
    std::thread* _pthilo1; //Atributo



  public:

    Caudalimetro();
    unsigned getNumeroPulsos();
    void resetNumeroPulsos();


};



#endif



