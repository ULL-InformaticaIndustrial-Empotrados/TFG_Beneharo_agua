// main() de la clase 'InteraccionUsuario'

#include "InteraccionUsuario.hpp"
#include "Caudalimetro.hpp"

#include <iostream>
#include <ctime> //std::strftime, std::time, ...
#include <chrono>
#include <thread>
#include <string>
#include <map>
#include <fstream> //librería para ficheros



int main() {

  std::string numLitros;
  std::string codigo;

  InteraccionUsuario iterac; //Creamos objeto de la clase 'InteraccionUsuario'
  Caudalimetro cau; //Objeto que actualiza el numero de litros
  Teclado tecl;



/*
  //Mas adelante realizar un fichero de texto con todas las claves y llamarlo
  //El fichero tiene que ser de la forma:
  //101 1234
  //102 2233
  //Y se lee como string
  codigos["101"] = "1234";
  codigos["102"] = "2233";
  codigos["103"] = "6657";
  codigos["104"] = "8923";
  codigos["105"] = "8887";
  codigos["201"] = "0089";
  codigos["202"] = "0349";
  codigos["203"] = "4382";
  codigos["204"] = "2190";
  codigos["205"] = "3970";
  codigos["301"] = "3372";
  codigos["302"] = "4509";
  codigos["303"] = "6503";
  codigos["304"] = "7721";
  codigos["305"] = "5576";
*/

  //Leer fichero viviendas-claves
  std::ifstream fichero("fclaves.txt");
  std::map<std::string, std::string> codigos;

  std::string vivienda;
  std::string clave;

  fichero >> vivienda;
  fichero >> clave;
  while(fichero.good()) {
    //std::cout << "La vivienda es '" << vivienda << "'" << std::endl;
    //std::cout << "El codigo es '" << clave << "'" << std::endl;

    codigos[vivienda] = clave;

    fichero >> vivienda;
    fichero >> clave;
  }



  //Máquina de estados 3 -- Correspondiente al número de intentos de vivienda y clave
  int estado = 0;


  while(1){

    switch(estado){


      case 0: //1º Pantalla -- Ejecutamos 'pantallaInicio'
        {
          iterac.pantallaInicio();
          estado = 1; //Pasamos al siguiente estado
        }
        break;


      case 1: //2º Pantalla -- Mostramos pantalla del numero de vivienda
        {
          int intentosVivienda = 0;

          do{
            vivienda = iterac.pantallaNumeroVivienda();
            intentosVivienda++;
          }while( (intentosVivienda < 3) && (codigos.find(vivienda) == codigos.end()) ); //codigos.find(vivienda) == codigos.end() es una comparación de string. Devuelve end()


          if( codigos.find(vivienda) == codigos.end() ){
            estado = 0;
            std::cerr << "Vivienda incorrecta." << std::endl;
          }else{
            estado = 2;
            std::cerr << "La vivienda " << vivienda << " es correcta." << std::endl;
          }
        }
        break;


      case 2: //3º Pantalla -- Mostramos pantalla codigo secreto, junto con el numero de vivienda
        {
          int intentosCodigo = 0;


          do{
            codigo = iterac.pantallaCodigoSecreto(vivienda);
            intentosCodigo++;
          }while( (intentosCodigo < 3) && (codigos[vivienda] != codigo) ); //Mientras intentos sean menores a 3 y el código de la vivienda sea distinto de su clave.


          if(codigos[vivienda] == codigo){
            estado = 3;
            std::cerr << "El código secreto " << codigo << " es correcto." << std::endl;
          }else{
            estado = 0;
            std::cerr << "Usuario se equivocó de código." << std::endl;
          }
        }
        break;


      case 3: //4º Pantalla -- Mostrar pantalla consumo de litros
        {
          std::string litros = iterac.pantallaConsumoLitros(vivienda);

          if(tecl.obtenerTecla() == '#'){
            estado = 4;
            std::cerr << "Saliendo." << std::endl;
          }
        }
        break;


      case 4: //Pantalla de adiós
        {
          std::string adios = iterac.adios();
          std::this_thread::sleep_for(std::chrono::milliseconds(3000));
          estado = 0;
        }
        break;

    }
  }


  return 0;
}


