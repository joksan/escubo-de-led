#include <SPI.h>
#include "cubo.h"

void setup() {
  randomSeed(0);
  inicializarCubo();
}

void loop() {
  //lluvia();
  //vomitoArcoiris();
  //planosDesplazando();
  //planoRotando();
  //aroDiagonal();

  todasLasAnimaciones();
/*
  llenarPlano(0, 0, magenta);
  llenarPlano(0, 1, magenta);
  llenarPlano(0, 2, magenta);
  llenarPlano(0, 3, magenta);
*/
  actualizarCubo();
}

void todasLasAnimaciones() {
  static int conteo = 0;
  static int animacion = 0;

  switch (animacion) {
    case 0: lluvia();            break;
    case 1: aroDiagonal();       break;
    case 2: vomitoArcoiris();    break;
    case 3: planoRotando();      break;
    case 4: planosDesplazando(); break;
  }

  conteo++;
  if (conteo >= 1000) {
    conteo = 0;
    limpiarCubo();
    animacion++;
    if (animacion >= 5)
      animacion = 0;
  }
}

void limpiarCubo() {
  byte i, j, k;

  //Simplemente barre los 3 ejes apagando cada LED
  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
      for (k=0; k<4; k++)
        escribirLED(i, j, k, 0);
}

void lluvia() {
  const int maxGotas = 8;
  static int posX[maxGotas];
  static int posY[maxGotas];
  static int posZ[maxGotas] = { -1, -1, -1, -1,
                                -1, -1, -1, -1
                              };
  static int velc[maxGotas];
  static int colr[maxGotas];
  int i;

  for (i = 0; i < maxGotas; i++) {
    //Verifica si la gota esta disponible para animar
    if (posZ[i] == -1) {
      //Si es asi, la inicializa en una columna al azar, en
      //la posicion mas alta, y con un color al azar
      posX[i] = random(0, 4);
      posY[i] = random(0, 4);
      posZ[i] = 0;
      velc[i] = random(8, 32);
      colr[i] = random(1, 8);
    }
    else {
      //Si la gota no esta ocupada, prosigue con la caida

      //Primero la borra de su posicion actual
      escribirLED(posX[i], posY[i], posZ[i] >> 8, 0);

      //Luego la avanza segun su velocidad
      posZ[i] += velc[i];

      //Verifica si alcanzo el fondo
      if ((posZ[i] >> 8) >= 4)
        //Si lo hizo, la marca como disponible
        posZ[i] = -1;
      else
        //Caso contrario, la redibuja
        escribirLED(posX[i], posY[i], posZ[i] >> 8, colr[i]);
    }
  }
}

void vomitoArcoiris() {
  //Simplemente cambia un led en una posicion al azar con
  //un color al azar en cada refresco
  escribirLED(random(0, 4), random(0, 4), random(0, 4),
              random(1, 8));
}

void planosDesplazando() {
  static byte etapa = 0;
  static byte paso = 0;
  static byte conteo = 0;

  //Llena los planos en 6 etpas, con colores diferentes
  //cada una. En cada ocasion se llenan los planos en la
  //direccion positiva de cada eje, alternandose luego con
  //las direcciones contrarias.
  switch (etapa) {
    case 0: llenarPlano(0, paso, rojo); break;
    case 1: llenarPlano(1, paso, verde); break;
    case 2: llenarPlano(2, paso, azul); break;
    case 3: llenarPlano(0, 3-paso, amarillo); break;
    case 4: llenarPlano(1, 3-paso, cyan); break;
    case 5: llenarPlano(2, 3-paso, magenta); break;
  }

  //Realiza un conteo de retardo modular, cambiando
  //primero los pasos (4 en total para llenar todo el cubo)
  //y luego las etapas (6 en total)
  conteo = (conteo < 19)? conteo+1: 0;
  if (conteo == 0) {
    paso = (paso < 3)? paso+1: 0;
    if (paso == 0) {
      etapa = (etapa < 5)? etapa+1: 0;
    }
  }
}

void planoRotando() {
  static byte conteo = 0;
  static byte paso = 0;
  static byte eje = 0;
  static byte color = 1;
  static byte ang = 0;

  //Primero borra el plano que se dibujo la ocasion anterior
  llenarPlanoRot(eje, ang, 0);

  //Nota: La funcion de dibujar planos con rotacion solo acepta 6
  //valores de angulos distintos (de 0 a 5), sin embargo esto solo
  //causa una rotacion de 180 grados, de manera que para causar
  //una rotacion completa se requieren 12 pasos de animacion
  //
  //El proceso de animacion lleva a cabo 1 vuelta completa sobre
  //cada eje, sin embargo para causar transiciones suaves entre eje
  //y eje se agregan 90 grados de rotacion adicionales, de forma que
  //el plano quede "centrado" sobre el siguiente, teniendose asi 15
  //pasos de animacion por eje.
  //
  //Curiosamente, el valor de angulo que queda del eje anterior
  //resulta ser precisamente el necesario que tenga el eje siguiente
  //para que la animacion "encaje", por asi decirlo. Esto resulta
  //conveniente para no tener que reinicializar la variable de
  //angulo cada vez que se cambia de eje.

  //Lleva a cabo un conteo modular de 10 frames entre pasos
  //de animacion
  conteo = (conteo < 9)? conteo+1: 0;
  if (conteo == 0) {
    //La animacion toma en total 45 pasos, 15 por cada eje
    paso = (paso < 44)? paso+1: 0;

    //El angulo tambien se incrementa en cada paso de animacion,
    //pero cuenta modularmente 6 pasos
    ang = (ang < 5)? ang+1: 0;

    //Cada vez que se repite la animacion reinicia el eje de rotacion
    //y el angulo al valor inicial
    if (paso == 0) {
      eje = 0;
      ang = 0;
      color = (color < 7)? color + 1: 1;
    }
    //Cuando se avanza al paso 15 y 30 se cambia de eje (pero no se
    //reinicializa el angulo)
    if (paso == 15) eje = 1;
    if (paso == 30) eje = 2;
  }

  //Finalmente dibuja el plano en la posicion actual
  llenarPlanoRot(eje, ang, color);
}

void aroDiagonal() {
  static byte conteo = 0;
  static byte paso = 0;
  static byte color = random(1, 8);
  byte posX, posY, posZ;
  char dirX, dirY, dirZ;
  byte i;

  //El dibujado se realiza como una suerte de aro
  //cuadrado, cuya posicion inicial esta determinada
  //por el paso de animacion, comenzando siempre desde
  //arriba en Z y avanzando hacia Z+
  posZ = 0; dirZ = 1;
  if (paso < 3) {
    //En los primeros 3 pasos, el dibujado se hara hacia X+
    posX = paso;      posY = 0;
    dirX = 1;         dirY = 0;
  }
  else if (paso < 6) {
    //En los siguientes 3 pasos, hacia Y+
    posX = 3;         posY = paso - 3;
    dirX = 0;         dirY = 1;
  }
  else if (paso < 9) {
    //En los siguientes 3, hacia X-
    posX = 9 - paso;  posY = 3;
    dirX = -1;        dirY = 0;
  }
  else {
    //En los ultimos 3, hacia Y-
    posY = 12 - paso; posX = 0;
    dirX = 0;         dirY = -1;
  }

  //Se incrementa la cuenta de espera, haciendo un conteo
  //modular
  conteo = (conteo < 19)? conteo+1: 0;
  if (conteo == 0) {
    //Cuando la cuenta cicla, se incrementa el paso en
    //form modular (de 0 a 11)
    paso = (paso < 11)? paso + 1: 0;
    if (paso == 0)
      //Cada vez que la animacion reinicia, se cambia el
      //color al azar
      color = random(1, 8);
  }

  //Limpia el cubo antes de dibujar
  limpiarCubo();
  for (i=0; i<12; i++) {
    //Dibuja el LED en la posicion actual
    escribirLED(posX, posY, posZ, color);

    //Avanza al siguiente pixel
    posX += dirX;
    posY += dirY;
    posZ += dirZ;

    //Si se avanzo totalmente a X+, se dirige a Y+
    if (dirX ==  1 && posX == 3) { dirX =  0; dirY =  1; }
    //Si se avanzo totalmente a Y+, se dirige a X-
    if (dirY ==  1 && posY == 3) { dirX = -1; dirY =  0; }
    //Si llego a X-, avanza a Y-
    if (dirX == -1 && posX == 0) { dirX =  0; dirY = -1; }
    //Si llego a Y-, reinicia hacia X+
    if (dirY == -1 && posY == 0) { dirX =  1; dirY =  0; }

    //Si se llego al fondo Z+, avanza hacia Z-
    if (dirZ ==  1 && posZ == 3) dirZ = -1;
    //Similarmente si se llego a la cima Z-, avanza a Z+
    if (dirZ == -1 && posZ == 0) dirZ =  1;
  }
}
