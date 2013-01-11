#include "timer.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

// Interrupt indicator, high at the start of ISR and low at the end.
const int interrupt_pin = 7;

// Oscillator output pin.
const int oscillator_pin = 11; // this will be a 3kHz square wave from the timer. (the carrier)
const int transmit_pin = ??; // this is the message to transmit
const int carrier_pin = 12; // this is the carrier input  (from the timer on pin 11).

const int mic_restart_pin = 8;

const int input_pin = ?;
const int transferRate = ?;
int unsigned transfer_time = 0;
byte buffer[10];

void setup()
{

    pinMode(mic_restart_pin, OUTPUT);
    pinMode(input_pin, INPUT);
    mySerial.begin(transferRate);

    // Setup the timer
    timer_set_prescale(PRESCALE_32);
    timer_set_mode(TIMER_CTC);

    timer_set_compare_a(85);
    timer_on_compare_a(TIMER_COMPARE_TOGGLE);

    // TODO: enable the timer interrupt
    bit_set(TIMSK2, OCIE2A);
}

void loop()
{
    
    if (input_pin == HIGH){
        digitalWrite(mic_restart_pin, HIGH);
        if (mySerial.available()){
            buffer = mySerial.read();
        }
        
        transmit_time = millis() + 1000
        for (i = 0; i < 5; i++){
            digitalWrite(transmit_pin, (buffer[i] & digitalRead(carrier_pin)));
        }
        
        while (millis() < start_time);
        
        for (i = 5; i < 10; i++){
            digitalWrite(transmit_pin, (buffer[i] & digitalRead(carrier_pin)));
        }
        
        digitalWrite(mic_restart_pin, LOW);
    } else {
      digitalWrite(mic_restart_pin, LOW);
    }
}
