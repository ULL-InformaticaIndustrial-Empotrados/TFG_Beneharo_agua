//Definición de los métodos de la clase 'Teclado'

#include "Teclado.hpp" //Incluye la clase 'BBB_I2C_Teclado'
#include <iostream>
#include <chrono>
#include <thread>


/* Para conectar los pines en otros sitios del GPIO de la BBB, por ejemplo al pasarle dicho trabajo a otro técnico
 * Teclado::Teclado(std::string nombreC1, std::string nombreC2, std::string nombreC3, std::string nombreF1
 * std::string nombreF2, std::string nombreF3, std::string nombreF4) : _c1(nombreC1), _c2(nombreC2), _c3("P8_38"), _f1("P8_40")
*/


//Constructor con los pines -- Inicializadores de los pines
Teclado::Teclado() : _c1("P8_34"), _c2("P8_36"), _c3("P8_38"), _f1("P8_40"),
  _f2("P8_42"), _f3("P8_44"), _f4("P8_46"){



	//Columnas -> Configuradas como salidas (out) y activadas (1)
  _c1.setDirection("out");
  _c1.setValue(1);


  _c2.setDirection("out");
  _c2.setValue(1);

  _c3.setDirection("out");
  _c3.setValue(1);



  //Filas configuradas como entradas (in), con 'pull-down' ('0' cuando está al aire)
  _f1.setDirection("in"); //Entrada
  _f1.setPullMode("gpio_pd"); //Modo usado -> pull-down (pd)
  _f1.setActiveLow(false); //Activar la lógica desordenada del pulsador pull_down para poner a '0'


  _f2.setDirection("in");
  _f2.setPullMode("gpio_pd");
  _f2.setActiveLow(false);


  _f3.setDirection("in");
  _f3.setPullMode("gpio_pd");
  _f3.setActiveLow(false);


  _f4.setDirection("in");
  _f4.setPullMode("gpio_pd");
  _f4.setActiveLow(false);
}


char Teclado::obtenerTecla(int msTimeout){


 //Con flancos
  BBB_GPIO_Interrupts intrr; //Objeto de la clase 'BBB_GPIO_Interrupts'

  //Añade los pines que interesan, las filas
  intrr.addPin(_f1);
  intrr.addPin(_f2);
  intrr.addPin(_f3);
  intrr.addPin(_f4);

  //"rising" -> flanco de subida
  _f1.setEdge("rising");
  _f2.setEdge("rising");
  _f3.setEdge("rising");
  _f4.setEdge("rising");


  char teclaPulsada = ' ';


  //Aseguramos que las columnas estén a '1' siempre antes del bucle, para evitar problemas
  _c1.setValue(1);
  _c2.setValue(1);
  _c3.setValue(1);

  intrr.edgeWait(msTimeout); //Espera sólo hasta que algunos de éstos flancos cambien

  if( (_f1.getValue() == 0) && (_f2.getValue() == 0)
      && (_f3.getValue() == 0) && (_f4.getValue() == 0) ){

        std::cerr << "Time out" << std::endl; //Mensaje de depuración 'fuera de tiempo'
        return 'T'; //Devuelve 'T' (fuera de tiempo) si no se ha pulsado ninguna tecla

  }


  std::this_thread::sleep_for(std::chrono::milliseconds(20)); //Esperamos 20 mseg para evitar los rebotes de los pulsadores

  //Columna 1
  _c1.setValue(1); //Poner 1ª columna a '1' y las demás a '0'
  _c2.setValue(0);
  _c3.setValue(0);

  if(_f1.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '1'" << std::endl;
    teclaPulsada = '1';

  }
  if(_f2.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '4'" << std::endl;
    teclaPulsada = '4';
  }
  if(_f3.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '7'" << std::endl;
    teclaPulsada = '7';
  }
  if(_f4.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '*'" << std::endl;
    teclaPulsada = '*';
  }


  //Columna 2
  _c1.setValue(0);
  _c2.setValue(1); //Poner 2ª columna a '1' y las demás a '0'
  _c3.setValue(0);

  if(_f1.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '2'" << std::endl;
    teclaPulsada = '2';
  }
  if(_f2.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '5'" << std::endl;
    teclaPulsada = '5';
  }
  if(_f3.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '8'" << std::endl;
    teclaPulsada = '8';
  }
  if(_f4.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '0'" << std::endl;
    teclaPulsada = '0';
  }


  //Columna 3
  _c1.setValue(0);
  _c2.setValue(0);
  _c3.setValue(1); //Poner 3ª columna a '1' y las demás a '0'

  if(_f1.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '3'" << std::endl;
    teclaPulsada = '3';
  }
  if(_f2.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '6'" << std::endl;
    teclaPulsada = '6';
  }
  if(_f3.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '9'" << std::endl;
    teclaPulsada = '9';
  }
  if(_f4.getValue() == 1 ){
    //std::cerr << "Se ha pulsado el '#'" << std::endl;
    teclaPulsada = '#';
  }



  //Bucle para salir de 'tecla pulsada'

  //Aseguramos que las columnas estén a '1' siempre antes del bucle, para evitar problemas
  _c1.setValue(1);
  _c2.setValue(1);
  _c3.setValue(1);

  //Finaliza cuando alguna fila esté a '1', es decir, alguna tecla se hubiese pulsado
  while( (_f1.getValue() == 1) || (_f2.getValue() == 1)
      || (_f3.getValue() == 1) || (_f4.getValue() == 1) ){

          std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  return teclaPulsada;
}




