#ifndef ticTimer_h
#define ticTimer_h

/*
OCR#A - Compare Match Register.
TCCRxB - Timer/Counter Control Register.(pre-scaler).
TCNTx - Timer/Counter Register. El valor actual del Timer se almacena aquí.
ICRx - Input Capture Register (solo para timers 16bit)
TIMSKx - Timer/Counter Mascara de interrupcion. Para habilitar/deshabilitar las interrupciones.
TIFRx - Timer/Counter Interrupt Flag Register. Indica una interrupcion pendiente.
*/

#include <inttypes.h>

class TicTimer{
private:
  uint32_t interruptFreq;
  uint16_t preescaler;
  uint32_t CompareMatchValue;
public:
  void set();
  void start();
  void stop();
};

#endif
