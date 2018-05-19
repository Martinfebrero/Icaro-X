#include <modulo.h>

#ifndef ACTUADOR_INCLUDED
#define ACTUADOR_INCLUDED
class Actuador : public Modulo {
public:
  Actuador(uint8_t pin) : Modulo(pin){};
  void move(uint8_t value){/*DO SOME*/};
  //void info(); // Sin estar declarada, se cumple que use el método "info()" de Modulo ya que hereda
};
#endif
