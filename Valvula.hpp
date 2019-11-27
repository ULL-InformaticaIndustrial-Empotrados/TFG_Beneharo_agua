
#ifndef _VALVULA_
#define _VALVULA_

#include "InteraccionUsuario.hpp"

#include "BBB_GPIO_pin.hpp"
#include "BBB_GPIO_Interrupts.hpp"


class Valvula {


  protected:
    BBB_GPIO_pin _v1;
    BBB_GPIO_pin _v2;
    unsigned numMillis = 0;



  public:

    Valvula();
    void abreValvula();
    void cierraValvula();



};




#endif




