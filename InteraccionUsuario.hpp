//Declaración de la clase 'InteraccionUsuario'

#ifndef _INTERACCIONUSUARIO_
#define _INTERACCIONUSUARIO_

#include "BBB_I2C_LCD.hpp"
#include "Teclado.hpp"

#include <string>


/*Es mejor que sea el programa principal el que cree los objetos LCD
  y Teclado, y el constructor de la clase que está creando los reciba como
  referencia y los guarde como 2 atributos, ya que, sólo puede haber un
  objeto LCD y Teclado en el programa. Seguro que los vas a necesitar en
  otras clases. Si los "creas" como atributos de esa clase después los
  tienes que "sacar" para que los usen otras clases.
*/


class InteraccionUsuario{

  protected: //Atributos
    BBB_I2C_LCD _lcd; //Atributo global de la clase 'LCD'
    Teclado _pul; //Atributo global de la clase 'Teclado'



  public: //Metodos

    InteraccionUsuario();

    void pantallaInicio();

    std::string pantallaNumeroVivienda();
    std::string pantallaCodigoSecreto(std::string vivienda); //Le pasamos como parámetro un string
    std::string pantallaNumeroPulsos(std::string vivienda);

};


#endif



