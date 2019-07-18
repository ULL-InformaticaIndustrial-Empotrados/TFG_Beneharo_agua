// int main() de la clase 'InteraccionUsuario'

#include "InteraccionUsuario.hpp"
#include <iostream>
#include <string>
#include <ctime> //std::strftime, std::time, ...
//#include <map.h> //pag. 515


int main() {


  //std::map<std::string, std::string> codigos;

  //codigos["001"]="1234";
  //codigos["002"]="2233";


  InteraccionUsuario iterac; //Creamos objeto de la clase 'InteraccionUsuario'

  iterac.pantallaInicio(); //Ejecutamos 'pantallaInicio'

  std::string vivienda = iterac.pantallaNumeroVivienda(); //Declaramos string 'vivienda'

  std::string codigo = iterac.pantallaCodigoSecreto(vivienda); //Mostrar pantalla codigo secreto junto con el string con el numero de vivienda

  std::cout << "La vivienda es: " << vivienda << std::endl;
  std::cout << "El codigo es: " << codigo << std::endl;

  //if ( codigos[vivienda] == codigo)



  return 0;
}

