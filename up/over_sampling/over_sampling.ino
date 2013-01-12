
/* Checks if thers is an incoming signal from the balloon. If thers is, the data is written to an array
   called "data" and this is used later in calculations. The arduino then calculates the checksum of the
   message and compares this to the checksum taken by the GPS arduino (in the balloon). If the checksums
   match, the message is interpreted into latitude, longitude and altitude and sends this to a computer
   via serial for it to be used in the pythin program.
*/

// Pin definitions
#define INPUT_PIN A0
#define MESSAGE_PIN 7
//#define POT_PIN A1
#define LAT_BITS 14
#define LONG_BITS 14
#define ALT_BITS 12
#define INT_BITS 16

//creates variables
int threshold = 511;           // The threshold at which the signal is classed as HIGH
int bit_limit = 48;            // The amount of bits that are allowed in the message.
int bit_rate = 40;             // Sets the bit rate of the transmission
int bit_delay = 1000/bit_rate; // Sets the standard delay for the bit reading
int over_sampler = (1000/bit_rate)/6;  // Sets the super sampling speed
int sample_result = 0;
int data_counter = 0;
int overSampler[5];

bool data[48];

int lng, lat, alt;

// reads the data from the "data" array and converts the bits to integers
void readDataBits (int* var, int startBit, int bitCount, bool* data, bool signedInt)
{
  *var = 0;
  // converts the data into two's complement
  if (data[startBit] == true && signedInt)
  {
    for ( int b = bitCount; b < INT_BITS; b++)
    {
      bitSet(*var, b);
    }
  }
  // Writing the bits into the byte
  for ( int c = 0; c < bitCount; c++)
  {
    bitWrite(*var, bitCount - 1 - c, (int)data[c + startBit]);
  }
}

void setup()
{
  pinMode(MESSAGE_PIN, OUTPUT); //sets up the message pin as an output
  Serial.begin(9600);           //sets up the serial conection to the computer
}

void loop()
{
  while (analogRead(INPUT_PIN) < threshold);
  delay (bit_delay);
  
  
  for (int bitCount = 0; bitCount < bit_limit; bitCount++)
  {
    //threshold = analogRead(POT_PIN);
    for (int sample = 0; sample < 5; sample++) {
      delay(over_sampler);
      overSampler[sample] = analogRead(INPUT_PIN);
    }
    
    sample_result = (overSampler[0] + overSampler[1] + overSampler[2] + overSampler[3] + overSampler[4]) / 5;
    
    if (sample_result >= threshold) {
      data[data_counter] = true;
    } else {
      data[data_counter] = false;
    }
    
    data_counter++;
    counter = 0;
  }
  
  // calculate checksum
  unsigned int a = 0;
  unsigned int b = 0;
  for (unsigned int i = 0; i < 10; i++)
  {
    // calculates the checksum of the blocks of data
    int nibble = data[i*4] * 8 +
      data[i*4 + 1] * 4 +
      data[i*4 + 2] * 2 +
      data[i*4 + 3];
    
    a += nibble;
    b += a;
  }
  
  a %= 16;
  b %= 16;
  
  // extracts the checksum of the received data
  int receivedA = data[8*4] * 8 +
      data[8*4 + 1] * 4 +
      data[8*4 + 2] * 2 +
      data[8*4 + 3];
  
  int receivedB = data[9*4] * 8 +
      data[9*4 + 1] * 4 +
      data[9*4 + 2] * 2 +
      data[9*4 + 3];
  
  if ((a == receivedA) && (b == receivedB)) // checks if the checksums match
  {
    readDataBits (&lat, 0, LAT_BITS, data, true);
    Serial.println(lat);
    Serial.print(",");
    
    readDataBits (&lng, LAT_BITS, LONG_BITS, data, true);
    Serial.println(lng);
    Serial.print(",");
    
    readDataBits (&alt, LAT_BITS + LONG_BITS, ALT_BITS, data, false);
    Serial.print(alt*10);
    Serial.print(",");

  }
}

