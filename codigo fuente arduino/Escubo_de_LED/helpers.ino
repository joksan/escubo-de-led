void llenarPlano(byte eje, byte pos, byte color) {
  byte i, j;

  //Se llena un plano de acuerdo al eje seleccionado
  switch (eje) {
    case 0:  //Sobre eje X, se llena plano YZ
      for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
          escribirLED(pos, i, j, color);
        }
      }
      break;
    case 1:  //Sobre eje Y, se llena plano XZ
      for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
          escribirLED(i, pos, j, color);
        }
      }
      break;
    case 2:  //Sobre eje Z, se llena plano XY
      for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
          escribirLED(i, j, pos, color);
        }
      }
      break;
  }
}

void llenarPlanoRot(byte eje, byte ang, byte color) {
  //Las siguientes tablas permiten generar una linea recta
  //empleando aritmetica modular, similar a lo que se hace
  //en el algoritmo de bressenham, sin embargo las posiciones
  //iniciales y los incrementos son determinados a plenitud
  //a traves del angulo de rotacion.
  //
  //Notese que el angulo 0 no causa que el plano se oriente
  //exactamente en angulo recto, ya que al hacerlo, su centro
  //estaria desviado del centro geometrico del cubo, por lo que
  //la posicion 0 agrega un pequeno angulo positivo de forma
  //que se logre centrar correctamente.
  //
  //Al avanzar los angulos en orden positivo se causan
  //rotaciones en la direccion de las manecillas del reloj.
  //Los angulos se cuentan desde 0 hasta 5, abarcando 180
  //grados. Dado que la rotacion de planos es simetrica, una
  //rotacion completa se lograria contando de 0 a 5 en 2
  //ocasiones, tomando 12 pasos de animacion.

  //El eje P seria el eje "horizontal", dependiendo de que cara
  //del cubo se mire (dependiendo del eje).
  const byte posP[6] = {  5,  6,  6,  6,  0,  2, };  //Posicion inicial
  const char incP[6] = { -1, -2, -2, -2,  2,  1, };  //Incremento

  //El eje Q seria el eje "vertical", dependiendo de la cara a
  //la que se aplique
  const byte posQ[6] = {  0,  0,  2,  5,  0,  0, };  //Posicion inicial
  const char incQ[6] = {  2,  2,  1, -1,  2,  2, };  //Incremento

  //Nota: Las coordenadas estan todas multiplicadas por 2, de forma
  //que se pueda crear el artificio matematico de que la posicion
  //incrementa en 0.5 posiciones. Esta es la razon por la cual las
  //coordenadas se desplazan un bit a la derecha (para dividir
  //entre 2) al llamar la funcion de escritura de LEDs.
  //
  //Notese tambien que las coordenadas estan incrementadas en 1
  //para cuando los incrementos son -1. Esto es porque el redondeo
  //es siempre hacia cero (truncado) cuando se divide entre 2.
  //De esta forma se consigue "ajustar" el punto de transicion de
  //la coordenada cerca del centro del cubo.

  byte posX, posY, posZ;
  byte i, j;

  //Se mapean los puntos del plano de textura PQ a los planos YZ,
  //XZ o XY, dependiendo del eje de rotacion
  switch (eje) {
    case 0:
      //Para el eje X se mapea el plano PQ sobre el plano YZ
      posY = posP[ang];
      posZ = posQ[ang];

      //Se dibuja la linea a lo largo de 4 pixeles sobre el
      //plano YZ
      for (i=0; i<4; i++) {
        //La linea se proyecta a lo largo del eje X, creando
        //un plano
        for (j=0; j<4; j++)
          escribirLED(j, posY>>1, posZ>>1, color);
        //Se incrementan las posiciones para el siguiente pixel
        //sobre YZ
        posY += incP[ang];
        posZ += incQ[ang];
      }
      break;
    case 1:
      //Se actua de forma similar para el eje Y proyectando PQ
      //sobre XZ
      posX = posP[ang];
      posZ = posQ[ang];

      for (i=0; i<4; i++) {
        for (j=0; j<4; j++)
          escribirLED(posX>>1, j, posZ>>1, color);
        posX += incP[ang];
        posZ += incQ[ang];
      }
      break;
    case 2:
      //Se actua de forma similar para el eje Z proyectando PQ
      //sobre XY
      posX = posP[ang];
      posY = posQ[ang];

      for (i=0; i<4; i++) {
        for (j=0; j<4; j++)
          escribirLED(posX>>1, posY>>1, j, color);
        posX += incP[ang];
        posY += incQ[ang];
      }
      break;
  }
}
