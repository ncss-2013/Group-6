#include "timer.h"

// Define pin numbers for ease of use
#define resetOut (8)
#define oscIn (2)
#define sqwvOut (11)
#define modOut (5)

void setup() {
	// Initiate pins to appropriate IO mode
	pinMode(resetOut, OUTPUT);
	pinMode(sqwvOut, OUTPUT);
	pinMode(modOut, OUTPUT);
	pinMode(oscIn, INPUT);

	// Set Timer2 to CTC mode, f/32 prescale
	timer_set_prescale(PRESCALE_32);
	timer_set_mode(TIMER_CTC);

	// Set Timer2 limit to 85, toggle mode
	timer_set_compare_a(85);
	timer_on_compare_a(TIMER_COMPARE_TOGGLE);

	// Enable timer interrupt 
	bit_set(TIMSK2, OCIE2A);
}

void loop() {
	//Nothing here (yet)
}

bool sqwvToggle = true; // Used to generate the 3KHz square wave
unsigned int resetCounter = 0; // Used to reset the microphone input occasionally to prevent timeout
 
ISR(TIMER2_COMPA_vect) {
	// Generate squarewave signal whatsit
	sqwvToggle = !sqwvToggle;

	// Write out squarewave if signal recieved, else flatline
	if (digitalRead(oscIn) == HIGH) {
		digitalWrite(modOut, sqwvToggle);
	} else {
		digitalWrite(modOut, LOW);
	}

	// Add to the reset counter, after ~20sec, reset the signal briefly.
	resetCounter ++;
	if (resetCounter > 60000) {
		resetCounter = 0;
		digitalWrite(resetOut, HIGH);
		digitalWrite(resetOut, LOW);
	}	
}