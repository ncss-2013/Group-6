#include "timer.h"

#define INTERRUPT_PIN 7
#define MODULATED 5
#define WAVE_OUT 11
#define WAVE_IN 12
#define LED 13
#define TRANSISTOR 8
//#define START_BYTE 97  //'a' easy to send over serial from PC for testing
#define START_BYTE 85
#define START_SENDING_BIT 1
#define SIGNAL_OUT 2

int c;
unsigned long count = 0;

byte data[7];

void setup()
{
    // Setup the output pins
    pinMode(INTERRUPT_PIN, OUTPUT);
    pinMode(WAVE_OUT, OUTPUT);
    pinMode(WAVE_IN, INPUT);
    pinMode(MODULATED, OUTPUT);
    pinMode(TRANSISTOR, OUTPUT);
    pinMode(LED, OUTPUT);
    pinMode(SIGNAL_OUT, OUTPUT);
    

    // Setup the timer
    timer_set_prescale(PRESCALE_32);
    timer_set_mode(TIMER_CTC);

    timer_set_compare_a(83);
    timer_on_compare_a(TIMER_COMPARE_TOGGLE);
    bit_set(TIMSK2, OCIE2A);
    
    Serial.begin(9600);
    data[0] = START_SENDING_BIT;
}

int current_byte=1;
int current_bit=0;
bool transmitting = false;
bool receiving = false;
byte send_bit=0;
//int data_received = 0;

void loop() 
{
  /*
  if (Serial.available()) {
    incoming_char = Serial.read()
    if (receiving) {
      data[data_received] = incoming_char;
      data_received++;
    }
    if (incoming_char == START_BYTE && !receiving) {
      receiving = true;
      turnRadioOn();
    }
  } */
  if (Serial.available() && !receiving) {
    if (Serial.read() == START_BYTE) {
      digitalWrite(TRANSISTOR, LOW);
      digitalWrite(LED, HIGH);
      receiving = true;
    }
  }
  if (Serial.available() && !transmitting) {
    data[current_byte] = Serial.read();
    current_byte++;
    if (current_byte >= 7) {
      transmitting = true;
      receiving = false;
      current_byte = 0;
    }
  }
  if (!Serial.available() && count >= 100) {
    count = 0;
    current_byte = 1;
    current_bit = 0;
    transmitting = false;
  }
  if (count >= 25 && transmitting) {
    send_bit = bitRead(data[current_byte], 7 - current_bit);
    Serial.print(send_bit);
    count = 0;
    current_bit++;
    if (current_bit >= 8) {
      current_bit = 0;
      current_byte++;
      Serial.print("\n");
    }
    if (current_byte >= 7) {
      current_byte = 1;
      send_bit = 0;
      transmitting = false;
      digitalWrite(MODULATED, LOW);
      digitalWrite(SIGNAL_OUT, LOW);
      digitalWrite(TRANSISTOR, HIGH);
      digitalWrite(LED, LOW);
    }
  }
  digitalWrite(MODULATED, send_bit && digitalRead(WAVE_IN));
  digitalWrite(SIGNAL_OUT, send_bit);

  //digitalWrite(MODULATED, digitalRead(WAVE_IN));
}

ISR(TIMER2_COMPA_vect) {
    // TODO: toggle interrupt_pin HIGH and then LOW
    c++;
    if (c == 6) {
      count++;
      c = 0;
    }
}
