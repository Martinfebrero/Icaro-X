#include <modulo.h>

/* Sensor es una abstracción de un sensor hardware,
   mantiene información básica necesaria de cualquier sensor y permite
   obtener su información de forma "cruda" así como puede determinar y notificar
   un suceso definido relacionado a sus valores, en ésta implementación se usa
   para pensar al sensor como un interruptor de encendido y apagado.

   lastM : Última medición tomada por el sensor.
   Calibre : Valor que toma el sensor estando en "reposo"-"sin activar"-"activación mínima"
   limite_max : Valor máximo que puede tomar el sensor.
   limite_min : Valor mínimo que puede tomar el sensor.

   rango : define un rango alrededor de "calibre", si ese rango se excede, se considera que el sensor está activo.
*/

#ifndef SENSOR_INCLUDED
#define SENSOR_INCLUDED

class Sensor : public Modulo {
private:
  int lastM;
  volatile uint16_t buffer [100];
  volatile uint8_t bufferCount=0;
  int16_t calibre;
  int16_t limite_max;
  int16_t limite_min;
  uint16_t rango;

public:
  Sensor(uint8_t);
  //volatile uint16_t buffer [100];
  void calibrar(void);
  int medir(void);
  void medirToBuffer(void);
  uint8_t getBufferCount();
  void setCalibreRango(uint16_t, uint16_t);
  void info();
};

#endif
