#ifndef Sensor_h
#define Sensor_h

#include <inttypes.h>

/*  Estructura que define un "Sensor" del sistema Icaro-X. Un Sensor tiene
   un "id" y un "pin" al que se conecta. El "id" hace referencia a la parte del
   cuerpo en que está posicionado.*/
struct Sensor {
  uint8_t id;
  uint8_t pin;
};

#endif
