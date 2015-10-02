#include <Arduino.h>

typedef union {
  byte num;
  struct {
    int r:1;
    int g:1;
    int b:1;
  };
} COLOR_RGB;

const int negro = 0;
const int rojo = 1;
const int verde = 2;
const int amarillo = 3;
const int azul = 4;
const int magenta = 5;
const int cyan = 6;
const int blanco = 7;

void inicializarCubo();
void escribirLED(byte x, byte y, byte z, byte color);
void actualizarCubo();
