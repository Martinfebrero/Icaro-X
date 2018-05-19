#include <Arduino.h>

#ifndef MODULO_INCLUDED
#define MODULO_INCLUDED

class Modulo {
private:
  uint8_t pin;
  uint8_t lock;

public:
  Modulo(uint8_t);
  void set_pin(uint8_t newpin) { this->pin = newpin; };
  uint8_t get_pin(void) { return this->pin; };
  uint8_t islocked(void) { return this->lock; };
  void changeLock(uint8_t value) { this->lock = value; };
  virtual void info(void);
};

#endif
