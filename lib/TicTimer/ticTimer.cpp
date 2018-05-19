#include <ticTimer.h>
#include <avr/interrupt.h>

void TicTimer::set(){
  cli();

  TCCR2A = 0; //Limpio registro
  TCCR2B = 0; //Limpio registro
  TIMSK2 = 0; //Limpio registro
  TCNT2 = 0;  //Counter Value a 0.

  //Set CompareMatchRegister calculado para la freq requerida. (100 Hz)
  OCR2A = 155;            // 155.25 = ((16*10^6) / (1024*100)) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);

  // Set CS2[2:0] bit for 1024 prescaler
  TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);

sei();//allow interrupts
};

void TicTimer::start(){
  cli();
  TIMSK2 |= (1 << OCIE2A);
  sei();
};

void TicTimer::stop(){
  cli();
  TIMSK2 &= ~(1<<OCIE2A);
  sei();
};
