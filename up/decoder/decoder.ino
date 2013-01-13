
/* Checks if thers is an incoming signal from the balloon. If thers is, the data is written to an array
   called "data" and this is used later in calculations. The arduino then calculates the checksum of the
   message and compares this to the checksum taken by the GPS arduino (in the balloon). If the checksums
   match, the message is interpreted into latitude, longitude and altitude and sends this to a computer
   via serial for it to be used in the pythin program.
*/

// Pin definitions
#define INPUT_PIN A0
#define MESSAGE_PIN 7
#define LAT_BITS 14
#define LONG_BITS 14
#define ALT_BITS 12
#define INT_BITS 16

//creates variables
int bit_limit = 48;            // The amount of bits that are allowed in the message.
int bit_rate = 40;             // Sets the bit rate of the transmission
int bit_delay = 1000/bit_rate; // Sets the standard delay for the bit reading

#define originLat (-121998)
#define originLng (544299)

bool data[48];

long lng, lat, alt;

// reads the data from the "data" array and converts the bits to integers
void readDataBits (long* var, int startBit, int bitCount, bool* data, bool signedInt)
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
  pinMode(13, OUTPUT);
  Serial.begin(9600);           //sets up the serial conection to the computer
}

bool readBit()
{
//  Serial.println(analogRead(INPUT_PIN));
  // The threshold at which the signal is classed as HIGH
  int threshold = 40;//analogRead(A1);
  int value = analogRead(INPUT_PIN);
  return value >= threshold;
}

void loop()
{
  digitalWrite(13, HIGH);
  while (!readBit());
  digitalWrite(13, LOW);
  delay (bit_delay/2);
  
  for (int counter = 0; counter < bit_limit; counter++)
  {
    delay(bit_delay);
    data[counter] = (readBit())? true : false;
    digitalWrite(7, data[counter]);
    Serial.print(int(data[counter]));
  }
  Serial.println("");
  
  // calculate checksum
  unsigned int a = 0;
  unsigned int b = 0;
  for (unsigned int i = 0; i < 10; i++)
  {
    int nibble = data[i*4] * 8 +
      data[i*4 + 1] * 4 +
      data[i*4 + 2] * 2 +
      data[i*4 + 3];
    
    a = (a + nibble) % 16;
    b = (b + a) % 16;
  }
  
  a %= 16;
  b %= 16;
  
  int receivedA = (data[10*4] << 3) +
      (data[10*4 + 1] << 2) +
      (data[10*4 + 2] << 1) +
      (data[10*4 + 3]);
  
  int receivedB = (data[11*4] << 3) +
      (data[11*4 + 1] << 2) +
      (data[11*4 + 2] << 1) +
      (data[11  *4 + 3]);
  
  bool allZeroes = true;
  for (unsigned int i = 0; (i < 48) && allZeroes; i++) {
    if (data[i] != 0)
      allZeroes = false;
  }
  
  if ((a == receivedA) && (b == receivedB) && !allZeroes)
  {
    readDataBits (&lat, 0, LAT_BITS, data, true);
    lat += originLat;
    Serial.print(lat);
    Serial.print(" = ");
    int lat_deg, lat_min, lat_sec;
    lat_deg = lat / 3600;
    lat %= 3600;
    if (lat_deg < 0) {
      lat_deg--;
      lat += 3600;
    }
    lat_min = lat / 60;
    lat_sec = lat % 60;
    Serial.print(lat_deg);
    Serial.print(' ');
    Serial.print(lat_min);
    Serial.print('\'');
    Serial.print(lat_sec);
    Serial.print("\" -- ");
    
    readDataBits (&lng, LAT_BITS, LONG_BITS, data, true);
    lng += originLng;
    Serial.print(lng);
    Serial.print(" = ");
    int lng_deg, lng_min, lng_sec;
    lng_deg = lng / 3600;
    lng %= 3600;
    if (lng_deg < 0) {
      lng_deg--;
      lng += 3600;
    }
    lng_min = lng / 60;
    lng_sec = lng % 60;
    Serial.print(lng_deg);
    Serial.print(' ');
    Serial.print(lng_min);
    Serial.print('\'');
    Serial.print(lng_sec);
    Serial.print("\" -- ");
    
    readDataBits (&alt, LAT_BITS + LONG_BITS, ALT_BITS, data, false);
    Serial.println(alt * 10);
  }
}

