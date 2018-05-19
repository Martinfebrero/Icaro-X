#include <modulo.h>

Modulo::Modulo(uint8_t pin) {
  this->pin = pin;
  this->lock = false;
};

void Modulo::info(void) {
  if (!Serial) {
    Serial.begin(9600);
  };
  Serial.print("\n* Modulo *\t*\n");
  Serial.print("Pin:\t");
  Serial.println(this->get_pin());
  Serial.print("Locked:\t");
  Serial.println(this->islocked());
  Serial.print("* *\t* *\t*\n");
  Serial.flush();
};
