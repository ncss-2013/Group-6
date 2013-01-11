/* Checks the incoming signal for if it is above a set threshold,
   finds the start bit and then prints the incoming signal as
   binary in the serial monitor.
   After this, the program will automatically ignore the end bit
   and revert to the original settings, wating for the next transmission
*/
#define INPUT_PIN A0
#define MESSAGE_PIN 7

// creates variables

int signal = 0;                // the incoming signal is kept here in binary

int threshold = 511;           // the threshold at which the signal is classed as HIGH

int signal_begin = 0;          // finds the beginning of the message. = 1 if the signal has been detected

int start_bit = 0;             // finds the start bit. = 1 if the start bit has been detected

int bit_count = 0;             // counts the information bits in the message. Will reset the message after it reaches the set limit

int bit_limit = 50;            // the amount of bits that are allowed in the message. will work in resetting the program and preparing it for the next transmission

int first_bit = 0 ;            // is used to change the delay for detection from the first bit level to the normal time. = 1 if the real message is being received


void setup()
{
  pinMode(MESSAGE_PIN, OUTPUT);

  Serial.begin(9600);


}

void loop()
{

  
  while (start_bit == 0 && signal_begin == 0)
  {
    analogRead(INPUT_PIN);

    signal = INPUT_PIN;

    if (signal >= threshold)
    {
      signal_begin = 1;
    }
  }

  while (start_bit == 0 && signal_begin == 1)
  {
    analogRead(INPUT_PIN);

    signal = INPUT_PIN;

    if (signal < threshold && start_bit == 1)
    {
      start_bit = 1;
    }
  }

  while (start_bit == 1 && signal_begin == 1 && first_bit == 0)
  {
    delay ((1000/80)*3);

    analogRead(INPUT_PIN);

    signal = INPUT_PIN;
    
    Serial.print((signal >= threshold) ? "1" : "0");

    first_bit = 1
  }
  
  while (start_bit == 1 && signal_begin == 1 && first_bit == 1)
  {
    delay (1000/40);
    
    analogRead(INPUT_PIN);
    
    signal = INPUT_PIN;
    
    Serial.print((signal >= threshold) ? "1" : "0");
  }
  
}









