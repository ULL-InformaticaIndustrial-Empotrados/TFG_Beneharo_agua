//Declaración de la clase 'InteraccionUsuario'

#ifndef _INTERACCIONUSUARIO_
#define _INTERACCIONUSUARIO_

#include "BBB_I2C_LCD.hpp"
#include "Teclado.hpp"
#include "Caudalimetro.hpp"
#include <string>




class InteraccionUsuario{

  protected:

    BBB_I2C_LCD _lcd; //Atributo global de la clase 'LCD'
    Teclado _pul; //Atributo global de la clase 'Teclado'





  public:

    InteraccionUsuario();

    void pantallaInicio();
    std::string pantallaNumeroVivienda();
    std::string pantallaCodigoSecreto(std::string vivienda); //Se muestra el numero de vivienda
    std::string pantallaConsumoLitros(std::string vivienda, Caudalimetro& cau); //Se muestra el numero de vivienda y litros (mediante objeto de la clase caudalimetro)
    void viviendaCorrecta();
    void viviendaIncorrecta();
    void claveIncorrecta();
    void tiempoLimite();
    std::string finSesion(std::string vivienda, Caudalimetro& cau);
    std::string adios();


};


#endif



