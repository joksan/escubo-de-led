#include <../../libraries/SPI/SPI.h>
#include "cubo.h"

byte frameBuffer[8][3];

const int pinCatodo[8] = { 9, 2, 6, 5, 8, 4, 7, 3 };
const int pinLatchCk = 10;

void inicializarCubo() {
  int i;

  //Se inicializan los pines de los catodos como salidas
  //en bajo
  for (i=0; i<8; i++) {
    pinMode(pinCatodo[i], OUTPUT);
    digitalWrite(pinCatodo[i], LOW);
  }

  //El pin de control de latch de los shift register se
  //inicializa como salida en alto
  pinMode(pinLatchCk, OUTPUT);
  digitalWrite(pinLatchCk, HIGH);

  //Inicializa el periferico SPI
  SPI.begin();
}

void escribirLED(byte x, byte y, byte z, byte color) {
  //El cubo de LED es fisicamente tridimensional, sin embargo
  //su topologia de conexion, asi como su organizacion en
  //memoria, son de caracter bidimensional.
  //
  //Se recomienda ver la fotografia en la documentacion del
  //programa para comprender mejor la topologia del cubo
  //de LED.
  //
  //A estas dimensiones las llamamos "planos" y "sectores",
  //entendiendose como planos las zonas gobernadas por los
  //catodos de los LED, que se encuentran organizadas
  //fisicamente como pares de filas intercaladas que se
  //extienden a lo largo del eje X, mientras que los
  //sectores son pares de LEDs que se encuentran en posiciones
  //adyacentes del eje Y, separando los 8 LEDs que componen
  //cada plano.
  //
  //Notese que las dimensiones fisicas son 4x4x4 = 64, y que las
  //dimensiones logicas son 8x8 = 64 tambien.

  //Se determina la ubicacion del plano y sector del LED en base
  //a sus coordenadas X, Y y Z
  byte plano = (z << 1) | (y & 1);
  byte sector = (x << 1) | (y >> 1);

  //Se hacen los traslados de los 3 bits del color a escribir a
  //sus posiciones correspondienes en la memoria de frame buffer,
  //segun el sector donde se encuentren
  switch (sector) {
    case 0:
      bitWrite(frameBuffer[plano][2], 7, bitRead(color, 2)); //B
      bitWrite(frameBuffer[plano][2], 6, bitRead(color, 1)); //G
      bitWrite(frameBuffer[plano][2], 5, bitRead(color, 0)); //R
      break;
    case 1:
      bitWrite(frameBuffer[plano][2], 3, bitRead(color, 2));
      bitWrite(frameBuffer[plano][2], 2, bitRead(color, 1));
      bitWrite(frameBuffer[plano][2], 1, bitRead(color, 0));
      break;
    case 2:
      bitWrite(frameBuffer[plano][2], 4, bitRead(color, 2));
      bitWrite(frameBuffer[plano][1], 7, bitRead(color, 1));
      bitWrite(frameBuffer[plano][1], 6, bitRead(color, 0));
      break;
    case 3:
      bitWrite(frameBuffer[plano][2], 0, bitRead(color, 2));
      bitWrite(frameBuffer[plano][1], 3, bitRead(color, 1));
      bitWrite(frameBuffer[plano][1], 2, bitRead(color, 0));
      break;
    case 4:
      bitWrite(frameBuffer[plano][1], 5, bitRead(color, 2));
      bitWrite(frameBuffer[plano][1], 4, bitRead(color, 1));
      bitWrite(frameBuffer[plano][0], 7, bitRead(color, 0));
      break;
    case 5:
      bitWrite(frameBuffer[plano][1], 1, bitRead(color, 2));
      bitWrite(frameBuffer[plano][1], 0, bitRead(color, 1));
      bitWrite(frameBuffer[plano][0], 3, bitRead(color, 0));
      break;
    case 6:
      bitWrite(frameBuffer[plano][0], 6, bitRead(color, 2));
      bitWrite(frameBuffer[plano][0], 5, bitRead(color, 1));
      bitWrite(frameBuffer[plano][0], 4, bitRead(color, 0));
      break;
    case 7:
      bitWrite(frameBuffer[plano][0], 2, bitRead(color, 2));
      bitWrite(frameBuffer[plano][0], 1, bitRead(color, 1));
      bitWrite(frameBuffer[plano][0], 0, bitRead(color, 0));
      break;
  }
}

void actualizarCubo() {
  int i, j;

  //Dibuja el cubo barriendo todos los planos
  for (i=0; i<8; i++) {
    //Luego barre los 3 shift register
    for (j=0; j<3; j++) {
      //Envia los datos mediante bus SPI
      SPI.transfer(frameBuffer[i][j]);
    }
    //Pulsa el latch clock para que se copien los datos
    digitalWrite(pinLatchCk, LOW);
    digitalWrite(pinLatchCk, HIGH);

    //Habilita el catodo del segmento
    digitalWrite(pinCatodo[i], HIGH);    

    //Espera un tiempo para que se los LED del segmento
    //se iluminen
    delayMicroseconds(1250);
    //delay(1000);

    //Deshabilit el catodo del segmento    
    digitalWrite(pinCatodo[i], LOW);
  }
}
