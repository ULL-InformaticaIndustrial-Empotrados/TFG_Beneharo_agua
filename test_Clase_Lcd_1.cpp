// main() de la clase 'InteraccionUsuario'

#include "InteraccionUsuario.hpp"
#include "Caudalimetro.hpp"
#include "Valvula.hpp"

#include <iostream>
#include <ctime> //std::strftime, std::time, ...
#include <chrono>
#include <thread>
#include <string>
#include <map>
#include <fstream> //librería para ficheros
#include <vector>
#include <iomanip> //std::setprecision, std::fixed


//Función que cierra la valvula mientras
void cerrarValvula(Caudalimetro& cau, Valvula& val){

  unsigned cuentas1;
  unsigned cuentas2;

  std::cout << "Intentando cerrar" << std::endl;
  val.abreValvula();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));


  do{
    val.cierraValvula();
    cuentas1 = cau.getNumeroPulsos();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cuentas2 = cau.getNumeroPulsos();

  }while(cuentas1 != cuentas2);
}


//Función que crea un fichero de texto, con el histórico de todas las sesiones
void litrosHistorico(std::string vivienda, Caudalimetro& cau){


  std::time_t t = std::time(nullptr);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%d/%m/%Y  %H:%M:%S", std::localtime(&t));
  std::cout << mbstr << '\n';



  float cuantosLitros = cau.getLitros();

  std::ofstream fout;
  fout.open("litrosHistorico.txt", std::ios::app); //Va añadiendo al final del .txt los consumos de las sesiones iniciadas


  if(fout.fail()){
    std::cout << "El archivo no se creó." << std::endl;

  }else{
    std::cout << "Se ha creado un archivo con el histórico." << std::endl;
    fout << vivienda << "  " << std::fixed << std::setprecision(4) //Escogemos los decimales a mostrar en el .txt
      << cuantosLitros << "   " << mbstr << "  " << std::endl;
  }

}




int main() {

  //std::string numLitros;
  std::string codigo;

  InteraccionUsuario iterac; //Creamos objeto de la clase 'InteraccionUsuario'
  Caudalimetro cau; //Objeto de la clase 'Caudalimetro' que actualiza el numero de litros
  Teclado tecl;
  Valvula val; //Definimos de nuevo el objeto 'val' de la clase 'Valvula'


  cerrarValvula(cau, val); //Llamada a la función



  //Leer fichero viviendas-claves
  std::ifstream fichero("fclaves.txt");
  std::map<std::string, std::string> codigos;

  std::string vivienda;
  std::string clave;

  fichero >> vivienda;
  fichero >> clave;

  while(fichero.good()) {

    codigos[vivienda] = clave;

    fichero >> vivienda;
    fichero >> clave;
  }





  //MÁQUINA DE ESTADOS PRINCIPAL
  //Máquina de estados 3 -- Funcionamiento general de las anteriores máquinas de estados en conjunto
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
          }while( (intentosVivienda < 1) && (codigos.find(vivienda) == codigos.end()) ); //codigos.find(vivienda) == codigos.end() es una comparación de string. Devuelve end()

          if(vivienda.size() == 0){
            estado = 0;


          }else if(codigos.find(vivienda) == codigos.end()){
            estado = 0;
            std::cerr << "Vivienda incorrecta!" << std::endl;
            iterac.viviendaIncorrecta();

          }else{
            estado = 2;
            std::cerr << "La vivienda " << vivienda << " es correcta." << std::endl;
            iterac.viviendaCorrecta();
          }

        }
        break;


      case 2: //3º Pantalla -- Mostramos pantalla codigo secreto, junto con el numero de vivienda
        {
          int intentosCodigo = 0;


          do{
            codigo = iterac.pantallaCodigoSecreto(vivienda);
            intentosCodigo++;
          }while( (intentosCodigo < 1) && (codigos[vivienda] != codigo) ); //Mientras intentos sean menores a 3 y el código de la vivienda sea distinto de su clave.

          if(codigo.size() == 0){
            estado = 0;


          }else if(codigos[vivienda] == codigo){
            estado = 3;
            std::cerr << "El código secreto " << codigo << " es correcto." << std::endl;

          }else{
            estado = 0;
            std::cerr << "Usuario se equivocó de código." << std::endl;
            iterac.claveIncorrecta();
          }
        }
        break;


      case 3: //4º Pantalla -- Mostrar pantalla consumo de litros
        {
          val.abreValvula(); //Al entrar en ésta pantalla, abrimos la válvula
          iterac.pantallaConsumoLitros(vivienda, cau);

          cerrarValvula(cau, val); //Si salimos de ésta pantalla, cerramos la válvula
          estado = 4;
          std::cerr << "Saliendo." << std::endl;
        }
        break;


      case 4: //Pantalla de confirmación de abandono de sesión
        {
          iterac.finSesion(vivienda, cau);

          std::this_thread::sleep_for(std::chrono::milliseconds(7000));
          estado = 5;
          std::cerr << "Fin de sesión." << std::endl;
        }
        break;


      case 5: //Pantalla de adiós
        {
          std::string adios = iterac.adios();
          litrosHistorico(vivienda, cau);
          std::this_thread::sleep_for(std::chrono::milliseconds(2000));
          estado = 0;
        }
        break;
    }
  }



  return 0; //Si todo OK
}


