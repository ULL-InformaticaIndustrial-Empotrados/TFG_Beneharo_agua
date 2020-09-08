
#ifndef _CAUDALIMETRO_
#define _CAUDALIMETRO_

#include "BBB_GPIO_pin.hpp"
#include "BBB_GPIO_Interrupts.hpp"
#include <thread>




//A la clase Caudalimetro el string(P8_36) se le pase como parámetro al constructor.


class Caudalimetro {


  protected:

    unsigned _nPulsos = 0; //Inicializada a 0 (apartir de c++11) y es unsigned ya que nunca va a tener valores negativos
    BBB_GPIO_Interrupts _intrr;//Objeto de la clase 'BBB_GPIO_Interrupts'
    void cuentaPulsos(); //Función privada
    std::thread* _pthilo1; //Puntero de tipo thread
    float pulsosPorLitro = 800; //Cantidad ya calculada a mano llenando una botella de 1 litro



  public:

    Caudalimetro();
    unsigned getNumeroPulsos(); //Lo que devuelve es unsigned (valor positivo siempre)
    float getLitros();
    void resetNumeroPulsos();


};



#endif



