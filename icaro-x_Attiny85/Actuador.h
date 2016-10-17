#ifndef Actuador_h
#define Actuador_h

#include <inttypes.h>

/*  Estructura que define un "Actuador" del sistema Icaro-X. Un actuador tiene
   un "id", un "npines" (número de pines) y un puntero a un arreglo de pines.
   También posee información de la posición actual.*/
struct Actuador {
  uint8_t id;
  uint8_t npines;
  uint8_t *arrayPines;
  uint8_t posicionActual;
};

#endif
