//Definición de los métodos de la clase 'InteraccionUsuario'

#include "InteraccionUsuario.hpp"
#include <iostream>
#include <chrono>


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
  _lcd.setCursor(0, 2); //En la fila 0, apartir de posicion 5
  _lcd.print("Num. vivienda: "); //Imprime
  _lcd.blink(); //Mostrar cursor parpadeante


  //Máquina de estados 1
  //Para el numero de vivienda. Implementado de 1 a 999 viviendas totales
  int estado = 0;
  std::string numeroVivienda;


  while(estado != 4){
      char tecla = _pul.obtenerTecla();
      //char tecla = _pul.obtenerTecla(int msTimeout = 15000); //En cualquier momento, si no se pulsa tecla tras 15 seg, retorna a pantalla 'adios'

      switch(estado){

        case 0: //Estado inicial
          if(tecla == '0' || tecla == '#' || tecla == '*'){
            estado = 0; //Nos quedamos en el estado inicial
            std::cerr << "Al principio no pulse " << tecla << std::endl;

          }
          else{
            estado = 1;
            numeroVivienda.push_back(tecla); //Almacena en el string
            _lcd.print(tecla); //Imprime por pantalla la tecla pulsada
          }
          break;

        case 1: //Estado 1 tecla
          if(tecla == '*')
            estado = 4; //Vamos al estado final
          else if(tecla == '#'){ //Si se pulsa '#' para borrar
            estado = 0; //Retrocede al estado anterior, que en éste caso es 0
            _lcd.moveCursorLeft(); //Mueve el cursor hacia la izquierda
            _lcd.print(" "); //Borra el caracter que había
            _lcd.moveCursorLeft(); //Deja el cursor sobre el borrado
            numeroVivienda.pop_back(); //Borra del string, el caracter borrado
          }
          else{
            estado = 2; //Avanza al estado siguiente
            numeroVivienda.push_back(tecla);
            _lcd.print(tecla); //Imprime tecla por el display
          }
          break;

        case 2: //Estado 2 teclas
          if(tecla == '*')
            estado = 4; //Vamos al estado final
          else if(tecla == '#'){
            estado = 1;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            numeroVivienda.pop_back();
          }
          else{
            estado = 3;
            numeroVivienda.push_back(tecla);
            _lcd.print(tecla);
          }
          break;

        case 3: //Estado 3 teclas
          if(tecla == '*')
            estado = 4; //Vamos al estado final
          else if(tecla == '#'){
            estado = 2;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            numeroVivienda.pop_back();
          }
          else
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

  //Vivienda seleccionada, al principio
  _lcd.setCursor(0, 0); //En la fila 0, apartir de posicion 0
  _lcd.print("Vivienda: ");
  _lcd.setCursor(10, 0);
  _lcd.print(vivienda.c_str()); //de string a char*, para imprimir por display lcd el char*


  //Clave secreta
  _lcd.setCursor(0, 2); //En la fila 0, apartir de posicion 5
  _lcd.print("Clave secreta: "); //Imprime
  _lcd.blink(); //Mostrar cursor parpadeante


  //Máquina de estados 2
  //Aqui hacemos lo mismo que en el caso anterior, pero ocultando la clave
  //con '*' para dejarla oculta
  int estado = 0;
  std::string numeroVivienda;


  while(estado != 5){
      char tecla = _pul.obtenerTecla();

      switch(estado){

        case 0: //Estado inicial
          if( tecla == '#' || tecla == '*'){
            estado = 0; //No escribimos nada
            std::cerr << "No pulse " << tecla << ". Introduce un entero"
              << std::endl;

          }
          else{
            estado = 1;
            numeroVivienda.push_back(tecla); //Almacena en el string
            _lcd.print('*'); //Imprime por pantalla la tecla pulsada
          }
          break;

        case 1: //Estado 1 tecla
          if(tecla == '*')
            estado = 1; //No escribimos nada
          else if(tecla == '#'){ //Si se pulsa '#' para borrar
            estado = 0; //Retrocede al estado anterior, que en éste caso es 0
            _lcd.moveCursorLeft(); //Mueve el cursor hacia la izquierda
            _lcd.print(" "); //Borra el caracter que había
            _lcd.moveCursorLeft(); //Deja el cursor sobre el borrado
            numeroVivienda.pop_back(); //Borra del string, el caracter borrado
          }
          else{
            estado = 2; //Avanza al estado siguiente
            numeroVivienda.push_back(tecla);
            _lcd.print('*'); //Imprime tecla por el display
          }
          break;

        case 2: //Estado 2 teclas
          if(tecla == '*')
            estado = 2; //No escribimos nada
          else if(tecla == '#'){
            estado = 1;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            numeroVivienda.pop_back();
          }
          else{
            estado = 3;
            numeroVivienda.push_back(tecla);
            _lcd.print('*');
          }
          break;

        case 3: //Estado 3 teclas
          if(tecla == '*')
            estado = 3; //No escribimos nada
          else if(tecla == '#'){
            estado = 2;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            numeroVivienda.pop_back();
          }
          else{
            estado = 4;
            numeroVivienda.push_back(tecla);
            _lcd.print('*');
          }
          break;

        case 4: //Estado 4 teclas
          if(tecla == '*')
            estado = 5; //Vamos al estado final
          else if(tecla == '#'){
            estado = 3;
            _lcd.moveCursorLeft();
            _lcd.print(" ");
            _lcd.moveCursorLeft();
            numeroVivienda.pop_back();
          }
          else
            estado = 4;
          break;
      }
  }



  return numeroVivienda;
}


// 4ª pantalla -- Pantalla numero de litros

//std::string InteraccionUsuario::pantallaConsumoLitros(std::string vivienda, Caudalimetro& cau){
std::string InteraccionUsuario::pantallaConsumoLitros(std::string vivienda){

  std::string litros;


  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd

  //Vivienda seleccionada, al principio
  _lcd.setCursor(0, 0); //En la fila 0, apartir de posicion 0
  _lcd.print("Vivienda: ");
  _lcd.setCursor(10, 0);
  _lcd.print(vivienda.c_str()); //de string a char*, para imprimir por display lcd el char*



  //Numero de litros
  _lcd.setCursor(0, 2); //En la fila 0, apartir de posicion
  _lcd.print("Litros: "); //Imprime "Litros: "
  _lcd.blink(); //Mostrar cursor parpadeante

  //Resetear el caudalimetro
  //Meterse en un bucle
  //Dentro del bucle, pedir los litros, actualizar litros en pantalla
  //Esperar tecla, durane 1 seg
  //Salir del bucle si la tecla es '*'


  return litros;
}



//Actualiza solo el número de litros, en la posicion especificada
std::string InteraccionUsuario::actualizaConsumo(){

  std::string numLitros;
/*
  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd

  _lcd.setCursor(8, 2); //Fila 2, posición 8
  _lcd.print("-");
*/

  return numLitros;
}


//Pantalla de despedida
std::string InteraccionUsuario::adios(){
  std::string mensaje;

  _lcd.clear();
  _lcd.begin();
  _lcd.backlight(); //Enciende la luz del lcd
  _lcd.setCursor(7, 2);
  _lcd.print("Adios.");

  return mensaje;
}






