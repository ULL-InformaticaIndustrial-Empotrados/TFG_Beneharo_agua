//Definición de los métodos de la clase 'InteraccionUsuario'

#include "InteraccionUsuario.hpp"
#include <iostream>
#include <chrono>
#include <string>
#include <sstream> //std::ostringstream
#include <iomanip> //std::setprecision, std::fixed



//Constructor con inicializador de los parámetros
InteraccionUsuario::InteraccionUsuario() : _lcd(1, 0x3f, 20, 4){

}


// Pantalla inicial de reposo
void InteraccionUsuario::pantallaInicio(){

  // 1ª pantalla
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd
  _lcd.setCursor(5, 0); //En la fila 0, apartir de posicion 5
  _lcd.print("Bienvenido"); //Imprime
  _lcd.setCursor(2, 3);
  _lcd.print("<Pulse una tecla>");


  do {
    //Fecha y hora en tiempo real
    std::time_t t = std::time(nullptr);
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), "%d/%m/%Y  %H:%M:%S", std::localtime(&t));
    std::cout << mbstr << '\n';

    _lcd.setCursor(0, 2);
    _lcd.print(mbstr); //Imprime fecha y hora actual

  } while( _pul.obtenerTecla(1000) == 'T'); //Mientras no se pulse 1 tecla durante 1 seg, haz lo del 'do'

}




// 2ª pantalla - 'Num. vivienda'
std::string InteraccionUsuario::pantallaNumeroVivienda(){

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd

  //Información para el usuario
  _lcd.setCursor(0, 3); //columna 0 y fila 3
  _lcd.print("(*)Entrar  (#)Borrar");

  _lcd.setCursor(0, 1); //Columna 0 y fila 1
  _lcd.print("Num. vivienda: "); //Imprime
  _lcd.blink(); //Mostrar cursor parpadeante



  //Máquina de estados 1
  //Para el numero de vivienda. Implementado de 1 a 999 viviendas totales
  int estado = 0;
  std::string numeroVivienda;


  while(estado != 4){


      //Time out de 15 seg. Desde que se deja de pulsar tecla durante 15 seg., sale a la pantalla de inicio
      char tecla = _pul.obtenerTecla(15000);

      std::cout << "La variable tiene valor " << estado << std::endl;



      switch(estado){


        //Estado inicial
        case 0:
          if(tecla == '0' || tecla == '#' || tecla == '*'){
            estado = 0; //Nos quedamos en el estado inicial
            std::cerr << "Al principio no pulse " << tecla << std::endl;

          } else if(tecla == 'T'){
              estado = 0;
              _lcd.clear();
              _lcd.setCursor(0, 2);
              _lcd.print("Tiempo excedido!");
              std::this_thread::sleep_for(std::chrono::milliseconds(3000));

              //std::string resultado();
              //return resultado();
              return std::string(); //Devolvemos string vacío

          } else {
                estado = 1;
                numeroVivienda.push_back(tecla); //Almacena en el string
                _lcd.print(tecla); //Imprime por pantalla la tecla pulsada
            }
            break;



        //Estado 1 tecla
        case 1:
          if(tecla == '*'){
            estado = 4; //Vamos al estado final

          } else if(tecla == '#'){ //Si se pulsa '#' para borrar
              estado = 0; //Retrocede al estado anterior, que en éste caso es 0 teclas
              _lcd.moveCursorLeft(); //Mueve el cursor hacia la izquierda
              _lcd.print(" "); //Borra el caracter que había
              _lcd.moveCursorLeft(); //Deja el cursor sobre el borrado
              numeroVivienda.pop_back(); //Borra del string, el último elemento

          } else if(tecla == 'T'){ //Si se recibe un Timeout, va hacia estado 0

              //En todos los estados si llega una T tendrías que salir del método devolviendo
              //un string vacío (o algo así) para indicar que el usuario no ha pulsado en
              //mucho tiempo. Y lo mismo en el resto de pantallas donde se pide algo
              //al usuario (el número de vivienda, la clave, ...)

              estado = 0;
              numeroVivienda.pop_back(); //Borra del string, el último elemento
              std::cerr << "No se ha pulsado tecla tras 15 seg." << std::endl;
              _lcd.clear();
              _lcd.setCursor(0, 2);
              _lcd.print("Tiempo excedido!");
              std::this_thread::sleep_for(std::chrono::milliseconds(3000));


              return std::string(); //Devolvemos string vacío

          } else{
              estado = 2; //Avanza al estado siguiente
              numeroVivienda.push_back(tecla);
              _lcd.print(tecla); //Imprime tecla por el display
          }
          break;




        //Estado 2 teclas
        case 2:
          if(tecla == '*')
            estado = 4; //Vamos al estado final
          else if(tecla == '#'){
            estado = 1;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            numeroVivienda.pop_back();

          } else if(tecla == 'T'){
              estado = 0;
              std::cerr << "No se ha pulsado tecla tras 15 seg." << std::endl;
              numeroVivienda.pop_back();
              numeroVivienda.pop_back(); //Borra del string, los 2 últimos elementos
              _lcd.clear();
              _lcd.setCursor(0, 1);
              _lcd.print("Tiempo excedido!");
              std::this_thread::sleep_for(std::chrono::milliseconds(3000));

              return std::string(); //Devolvemos string vacío

            } else{
                estado = 3;
                numeroVivienda.push_back(tecla);
                _lcd.print(tecla);
              }
              break;




        //Estado 3 teclas
        case 3:
          if(tecla == '*')
            estado = 4; //Vamos al estado final
          else if(tecla == '#'){
            estado = 2;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            numeroVivienda.pop_back();

          } else if(tecla == 'T'){
              estado = 0;
              std::cerr << "No se ha pulsado tecla tras 15 seg." << std::endl;
              numeroVivienda.pop_back();
              numeroVivienda.pop_back();
              numeroVivienda.pop_back(); //Borra del string, los 3 últimos elementos
              _lcd.clear();
              _lcd.setCursor(0, 1);
              _lcd.print("Tiempo excedido!");
              std::this_thread::sleep_for(std::chrono::milliseconds(3000));

              return std::string(); //Devolvemos string vacío

            } else
                estado = 3;
              break;
      }

  }
  return numeroVivienda;
}




// 3ª pantalla -- 'Clave secreta'
std::string InteraccionUsuario::pantallaCodigoSecreto(std::string vivienda){ //con un string como parámetro

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd

  //Información para el usuario
  _lcd.setCursor(0, 3); //En la columna 0 y fila 3
  _lcd.print("(*)Entrar  (#)Borrar");

  //Vivienda seleccionada, al principio
  _lcd.setCursor(0, 0); //Columna 0 y fila 0
  _lcd.print("Vivienda: ");
  _lcd.setCursor(10, 0);
  _lcd.print(vivienda.c_str()); //de string a char*, para imprimir por display lcd el char*


  //Clave secreta
  _lcd.setCursor(0, 1); //En la columna 0 y fila 1
  _lcd.print("Clave secreta: "); //Imprime
  _lcd.blink(); //Mostrar cursor parpadeante



  //Máquina de estados 2
  //Aqui hacemos lo mismo que en el caso anterior, pero ocultando la clave
  //con '*' para dejarla oculta
  int estado = 0;
  std::string codigoVivienda;


  while(estado != 5){

      //Time out (15 segundos) desde que se deja de pulsar tecla, sale a la pantalla de inicio
      char tecla = _pul.obtenerTecla(15000);

      std::cout << "Estoy en estado " << estado << " y tecla " << tecla << std::endl;

      switch(estado){



        //Estado inicial
        case 0:
          if( tecla == '#' || tecla == '*'){
            estado = 0; //No escribimos nada
            std::cerr << "No pulse " << tecla << ". Introduce un entero"
              << std::endl;

          } else if(tecla == 'T'){
              estado = 0;
              std::cerr << "No se ha pulsado tecla tras 15 seg." << std::endl;
              _lcd.clear();
              _lcd.setCursor(0, 2);
              _lcd.print("Tiempo excedido!");
              std::this_thread::sleep_for(std::chrono::milliseconds(3000));

              return std::string(); //Devolvemos string vacío

          } else{
            estado = 1;
            codigoVivienda.push_back(tecla); //Almacena en el string
            _lcd.print('*'); //Imprime por pantalla la tecla pulsada
          }
          break;




        //Estado 1 tecla
        case 1:
          if(tecla == '*')
            estado = 5; //No escribimos nada

          else if(tecla == '#'){ //Si se pulsa '#' para borrar
            estado = 0; //Retrocede al estado anterior, que en éste caso es 0
            _lcd.moveCursorLeft(); //Mueve el cursor hacia la izquierda
            _lcd.print(" "); //Borra el caracter que había
            _lcd.moveCursorLeft(); //Deja el cursor sobre el borrado
            codigoVivienda.pop_back(); //Borra del string, el caracter borrado

          } else if(tecla == 'T'){
              estado = 0;
              std::cerr << "No se ha pulsado tecla tras 15 seg." << std::endl;
              codigoVivienda.pop_back(); //Borra del string, el último elemento
              _lcd.clear();
              _lcd.setCursor(0, 2);
              _lcd.print("Tiempo excedido!");
              std::this_thread::sleep_for(std::chrono::milliseconds(3000));

              return std::string(); //Devolvemos string vacío


            } else{
                estado = 2; //Avanza al estado siguiente
                codigoVivienda.push_back(tecla);
                _lcd.print('*'); //Imprime tecla por el display
              }
              break;




        //Estado 2 teclas
        case 2:
          if(tecla == '*'){
            estado = 2; //No escribimos nada

          } else if(tecla == '#'){
              estado = 1;
              _lcd.moveCursorLeft();
              _lcd.print(" ");
              _lcd.moveCursorLeft();
              codigoVivienda.pop_back();

            } else if(tecla == 'T'){
                estado = 0;
                std::cerr << "No se ha pulsado tecla tras 15 seg." << std::endl;
                codigoVivienda.pop_back();
                codigoVivienda.pop_back(); //Borra las 2 últimas teclas del string
                _lcd.clear();
                _lcd.setCursor(0, 2);
                _lcd.print("Tiempo excedido!");
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));

                return std::string(); //Devolvemos string vacío

              } else{
                  estado = 3;
                  codigoVivienda.push_back(tecla);
                  _lcd.print('*');
                }
                break;




        //Estado 3 teclas
        case 3:
          if(tecla == '*')
            estado = 3; //No escribimos nada

          else if(tecla == '#'){
            estado = 2;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            codigoVivienda.pop_back();

          } else if(tecla == 'T'){
                estado = 0;
                std::cerr << "No se ha pulsado tecla tras 15 seg." << std::endl;
                codigoVivienda.pop_back();
                codigoVivienda.pop_back();
                codigoVivienda.pop_back(); //Borra las 3 últimas teclas del string
                _lcd.clear();
                _lcd.setCursor(0, 2);
                _lcd.print("Tiempo excedido!");
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));

                return std::string(); //Devolvemos string vacío

            } else{
                estado = 4;
                codigoVivienda.push_back(tecla);
                _lcd.print('*');
              }
              break;




        //Estado 4 teclas
        case 4:
          if(tecla == '*')
            estado = 5; //Vamos al estado final

          else if(tecla == '#'){
            estado = 3;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            codigoVivienda.pop_back();

          } else if(tecla == 'T'){
                estado = 0;
                std::cerr << "No se ha pulsado tecla tras 15 seg." << std::endl;
                codigoVivienda.pop_back();
                codigoVivienda.pop_back();
                codigoVivienda.pop_back();
                codigoVivienda.pop_back(); //Borra las 4 últimas teclas del string
                _lcd.clear();
                _lcd.setCursor(0, 2);
                _lcd.print("Tiempo excedido!");
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));

                return std::string(); //Devolvemos string vacío

            } else
                estado = 4;
              break;
      }
  }



  return codigoVivienda;
}


// 4ª pantalla -- Pantalla numero de litros

std::string InteraccionUsuario::pantallaConsumoLitros(std::string vivienda, Caudalimetro& cau){


  std::string litros;



  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd


  //Información para el usuario
  _lcd.setCursor(0, 3); //En la fila 0, apartir de posicion 0
  _lcd.print("(*)Salir  (#)LtTotal");


  //Vivienda seleccionada, al principio
  _lcd.setCursor(0, 0); //En la fila 0, apartir de posicion 0
  _lcd.print("Vivienda: ");
  _lcd.setCursor(10, 0);
  _lcd.print(vivienda.c_str()); //de string a char*, para imprimir por display lcd el char*



  //Numero de litros
  _lcd.setCursor(0, 1); //En la fila 0, apartir de posicion
  _lcd.print("Litros: "); //Imprime "Litros: "
  _lcd.blink(); //Mostrar cursor parpadeante



  //Resetear el caudalimetro
  //Meterse en un bucle
  //Dentro del bucle, pedir los litros, actualizar litros en pantalla
  //Esperar tecla, durante 1 seg
  //Salir del bucle si la tecla es '*'




  cau.resetNumeroPulsos(); //Reseteamos el nº de pulsos en cada inicio de sesión

  do{

    float cuantosLitros = cau.getLitros();
    std::ostringstream oss;

    oss << std::setw(7) << std::fixed << std::setprecision(3) << cuantosLitros;

    _lcd.setCursor(8, 1); //Columna 8 y fila 1
    _lcd.print(oss.str().c_str());

    std::cout << "Litros: " << oss.str() << std::endl;

  }while(_pul.obtenerTecla(1000) != '*'); //Mientras se espere 1 seg y no se pulse '*'


  return litros;
}


//Pantalla de confirmación de salir de sesión
std::string InteraccionUsuario::finSesion(std::string vivienda, Caudalimetro& cau){

  //std::string litrosTotal;

  float cuantosLitros = cau.getLitros();
  std::ostringstream oss;

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd

  //Vivienda de la última sesión
  _lcd.setCursor(0, 0); //En la fila 0, apartir de posicion 0
  _lcd.print("Vivienda: ");
  _lcd.setCursor(10, 0);
  _lcd.print(vivienda.c_str()); //de string a char*, para imprimir por display lcd el char*

  //Litros acumulados de la última sesión
  _lcd.setCursor(0, 2);
  _lcd.print("Litros: ");

  oss << std::setw(5) << std::fixed << std::setprecision(3) << cuantosLitros;
  _lcd.setCursor(8, 2);
  _lcd.print(oss.str().c_str());


  //return litrosTotal;
  return oss.str().c_str();
}




void InteraccionUsuario::viviendaCorrecta(){

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd
  _lcd.setCursor(1, 2);
  _lcd.print("Vivienda correcta!");
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

}


void InteraccionUsuario::viviendaIncorrecta(){

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd
  _lcd.setCursor(1, 2);
  _lcd.print("Vivienda no existe!");
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}



void InteraccionUsuario::claveIncorrecta(){

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd
  _lcd.setCursor(1, 2);
  _lcd.print("Clave incorrecta!");
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}


void InteraccionUsuario::tiempoLimite(){

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd
  _lcd.setCursor(1, 2);
  _lcd.print("Tiempo superado!");
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}




//Pantalla de despedida
std::string InteraccionUsuario::adios(){

  std::string mensaje;

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd
  _lcd.setCursor(7, 2);
  _lcd.print("Adios.");
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  return mensaje;
}

