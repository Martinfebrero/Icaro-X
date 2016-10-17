#ifndef xmodulo_h
#define xmodulo_h

#include "Actuador.h"
#include "Sensor.h"

#include <inttypes.h>

/*Estructura que determina un módulo funcional "xmodulo" del sistema Icaro-X.
Cada módulo tiene un array de sensores y de actuadores, falta definir un máximo.
También posee una timeoutFlag para determinar si llegó al timeout.
El "id" de los sensores y actuadores define el "id" del xmodulo, osea que un
módulo sólo actuará si uno de sus sensores o actuadores debe hacerlo.
También posee el último mensaje recibido.
*/
struct Xmodulo {
  volatile uint8_t timeout;
  struct Actuador *arrayActuadores;
  struct Sensor *arraySendores;
};

#endif
