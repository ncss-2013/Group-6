#include "timer.h"

// Interrupt indicator, high at the start of ISR and low at the end.
const int interrupt_pin = 7;

// Oscillator output pin.
const int oscillator_pin = 11;
const int signal_pin = 2;
const int signal_receive = 12;
const int victim = 5;
const int theMaster = 8;
int state = 0;
int reset = 0;
void setup()
{
    // Setup the output pins
    pinMode(interrupt_pin, OUTPUT);
    pinMode(oscillator_pin, OUTPUT);
    pinMode(signal_pin, INPUT);
    pinMode(signal_receive, INPUT);
    pinMode(victim, OUTPUT);
    pinMode(theMaster, OUTPUT);
    

    // Setup the timer
   
    timer_set_prescale(PRESCALE_1024);
    timer_set_mode(TIMER_CTC);

    timer_set_compare_a(6);
    timer_on_compare_a(TIMER_COMPARE_TOGGLE);

    // TODO: enable the timer interrupt
    bit_set( TIMSK2, OCIE2A);
}



void loop()
{

    
  
    
  // Nothing to do here.
    // The oscillator is produced by the hardware.

}

ISR(TIMER2_COMPA_vect) {
    // TODO: toggle interrupt_pin HIGH and then LOW
    digitalWrite(7, HIGH);
    digitalWrite(7, LOW);
    int signal = (digitalRead(signal_pin));
    
    if (signal == 1)
    {
      digitalWrite(victim, state);
      state = !state;
      
    }
    else
    {
      digitalWrite(victim, LOW);
      
    }
    

    if (reset == (344*3))
    {
      digitalWrite (theMaster, HIGH);
      delayMicroseconds(20);
      digitalWrite(theMaster, LOW);
      
      reset = 0;
    }
    else
    {
      reset++;
    }
    
}

