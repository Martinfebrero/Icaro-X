#include <actuador.h>
#include <sensor.h>

#ifndef CONTROLADOR_DEFINED
#define CONTROLADOR_DEFINED

class Controlador {
private:
  uint8_t nSensores;
  uint8_t nActuadores;
  Sensor *sensores;
  Actuador *actuadores;
  uint8_t lock;

public:
  Controlador(Sensor *sensores, uint8_t nSensores, Actuador *actuadores,
              uint8_t nActuadores);
  void activar();
  void (Controlador::*sensar)();
  void changeLock(uint8_t value) { this->lock = value; }
  void info();

/*
  void set_timer(unsigned long);
  void start_timer();
  void stop_timer();
  */
};
#endif
