#include <SoftwareSerial.h>

// The longitude & latitude values, written to by parseLongLatitude().
int latDeg, latMin, latSec;    // Declare the latitude degree, minute and second
int lonDeg, lonMin, lonSec;    // Declare the longitude degree, minute and second
char latDir, lonDir;           // Declare the latitude and longitude directions (South(S), East(E), etc)
int speed;                     // Declare the speed (in knots)
int found = 0;

// Declare a software serial port to interface with the GPS unit.
// The receive and transmit pins are 8 and 9 respectively.
SoftwareSerial GPSSerial(8, 9); // RX, TX

void setup()
{
  // Initialise hardware serial port for communication with the PC
  Serial.begin(9600);

  // Initialise software serial port for communication with the GPS
  // The default speed on the GPS is 4800 baud.
  GPSSerial.begin(4800);
}

void loop()
{
  if (readGPSData()) {
    //    Serial.print("Longitude: ");
    //    Serial.print(lonDeg);
    //    Serial.print(" Degrees");
    //    Serial.print(lonMin);
    //    Serial.print(" Minutes");
    //    Serial.print(lonSec);
    //    Serial.print(" Seconds");
    //    Serial.println(lonDir);

    //    Serial.print("Latitude: ");
    //    Serial.print(latDeg);
    //    Serial.print(" Degrees");
    //    Serial.print(latMin);
    //    Serial.print(" Minutes");
    //    Serial.print(latSec);
    //    Serial.print(" Seconds");
    //    Serial.println(latDir);
    //
    //    Serial.print("Speed: ");
    //    Serial.print(speed);
    //    Serial.println(" knots.");
  }
}

bool readGPSData()
{
  // Read a byte from the GPS receiver
  if (readChar() != '$') {
    return false;
  }

  // Ignore the first two bytes which are always "GP".
  if (!(readChar() == 'G' && readChar() == 'P')) {
    return false;
  }

  // Read the next three bytes which specify the header
  /* TODO: Check if this is an 'RMC' sentence */

  if ((readChar() == 'R' && readChar() == 'M' && readChar() == 'C' && found == 0))
  {
    Serial.println("8Found RMC sentence");	// For testing
    found++;
  }

  if (readChar() != ',') {
    return false;
  }

  // Skip the timestamp
  while (readChar() != ',') {
    /* Do Nothing */
  }


  /* TODO: Check if we have a gps fix */

  if ((readChar() != 'A' ))
  { 
    return false;

  }
  if (readChar() != ',') {
    return false;
  }
  /* TODO: Parse Latitude and Longitude */
  Serial.print("Latitude: ");
  latDeg = (readChar() && readChar());
  Serial.print(latDeg);
  Serial.print(" Degrees");
  latMin = (readChar() && readChar());
  Serial.print(latMin);
  Serial.print(" Minutes");
  if (readChar() != '.') 
  {
    return false;
  }

  latSec = ((readChar() && readChar() && readChar() && readChar())*60);

  if (latSec > 59.99)
  {
    latSec = (latSec - 60);
    latMin++;
  }
  Serial.print(latSec);
  Serial.print(" Seconds");



//Serial.println(readChar());

  if (readChar() != ',') 
  {
    return false;
  }

  latDir = (readChar());
  Serial.println(latDir);
  if (readChar() != ',')
  {
    return false;
  }
  Serial.print("Longitude: ");
  lonDeg = (readChar() && readChar() && readChar());
  Serial.print(lonDeg);
  Serial.print(" Degrees");
  lonMin = (readChar() && readChar());
  Serial.print(lonMin);
  Serial.print(" Minutes");
  if (readChar() != '.') 
  {
    return false;
  }

  lonSec = ((readChar() && readChar() && readChar() && readChar())*60);

  if (lonSec < 59.99)
  {
    lonSec = (lonSec - 60);
    lonMin++;
  }

  Serial.print(lonSec);
  Serial.print(" Seconds"); 




  if (readChar() != ',')
  {
    return false;
  }

  lonDir = (readChar());
  Serial.println(lonDir);
  if (readChar() != ',')
  {
    return false;
  }


  /* TODO: Parse speed */

  speed = (readChar() && readChar() && readChar() && readChar() && readChar());


  Serial.print("Speed: ");
  Serial.print(speed);
  Serial.println(" knots.");

  return true;
}

// Define a function to read a byte from the GPS receiver
char readChar()
{
  int c;
  while ((c = GPSSerial.read()) == -1)
    /* Do Nothing */    ;

  return c;
}

// Define the function to read the long/lat
bool parseLongLatitude(bool isLongitude)
{
  int deg, min, sec;
  char dir;

  // The format of the information is:
  //   Longitude: dddmm.mmmmm
  //   Latitude:   ddmm.mmmmm

  /* TODO: Read the degrees */
  /* TODO: Read the minutes/seconds */

  // Note: the ',' at the end of the lon/lat has already been consumed

  // Read the latitude direction
  dir = readChar();

  // Save the parsed results.
  if (isLongitude) {
    lonDeg = deg;
    lonMin = min;
    lonSec = sec;
    lonDir = dir;
  } 
  else {
    latDeg = deg;
    latMin = min;
    latSec = sec;
    latDir = dir;
  }

  return true;
}

float readFloatUntilComma(void) {
  /* read a floating point number (something of the form of 123.4567,) */
  return 0.0;
}

bool isDigit(char c) {
  /* Is the character c within the ascii range 0-9 */
  return false;
}

// Read in a 'num_chars' integer from serial.
int readInt(int num_chars) {
  return 0;
}



