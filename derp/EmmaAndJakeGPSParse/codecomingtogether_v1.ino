#include <SoftwareSerial.h>
#include <string.h>


// Declare a software serial port to interface with the GPS unit.
// The receive and transmit pins are 8 and 9 respectively.
SoftwareSerial GPSSerial(8, 9); // RX, TX

latlon lat(0,0,0);
latlon lon(0,0,0);


//Container for split GPS data strings
char * GPSinfo[200];

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
    Serial.print("Longitude: ");
    Serial.print(lonDeg);
    Serial.print(" ");
    Serial.print(lonMin);
    Serial.print("' ");
    Serial.print(lonSec);
    Serial.print("\" ");

    Serial.print("Latitude: ");
    Serial.print(latDeg);
    Serial.print(" ");
    Serial.print(latMin);
    Serial.print("' ");
    Serial.print(latSec);
    Serial.print("\" ");


   
  }
}

//works out what type of GPS data it is
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
 if (!(readChar() == 'R' && readChar() == 'M' && readChar() == 'C'))
 {
  return false;
 }
  Serial.println("Found RMC sentence");	// For testing

  if (readChar() != ',') {
    return false;
  }

  // Skip the timestamp
  while (readChar() != ',') {
    /* Do Nothing */
  }

  /* Check if we have a gps fix */
  if (!(readChar() == 'A'))
  {
    Serial.println("Fell out without active GPS");
    return false;
  }

  if (readChar() != ',')
  {
    return false;
  }

  char line[1500];
  int length = GPSSerial.readBytesUntil('\r', line, 22);
  line[length] = 0;
  
  char * pch;
  int i = 0;
  
  // First call as required, just initializes the split   
  strtok( line, "," );
     
  while (pch != NULL)
  {
    GPSinfo[i] = pch;
    pch = strtok (NULL, ",");
     
       
    i++;
  }

  



  /* Parse Latitude and Longitude */
  parseLongLatitude(false);
  parseLongLatitude(true);



  return true;
}

// Define a function to read a byte from the GPS receiver
char readChar()
{
  int c;
  while ((c = GPSSerial.read()) == -1)
    /* Do Nothing */;

  return c;
}

// Define the function to read the long/lat
bool parseLongLatitude(bool isLongitude, latlon l, int indexInGPS)
{
  char str[200];
  
  
  if (isLongitude)
  {
    strcat(str, GPSinfo[indexInGPS[0]]);
    strcat(str, GPSinfo[indexInGPS[1]]);
    strcat(str, GPSinfo[indexInGPS[2]]);
    
    l.degs = atoi(str);
    
    str[0] = 0;
    
    strcat(str, GPSinfo[indexInGPS[3]]);
    strcat(str, GPSinfo[indexInGPS[4]]);
    
    l.mins = atoi(str);
    
    str[0] = 0;
    
    strcat(str, GPSinfo[indexInGPS[6]]);
    strcat(str, GPSinfo[indexInGPS[7]]);
    strcat(str, GPSinfo[indexInGPS[8]]);
    
    l.secs = atoi(str) * 60;
  
  }
  else
  {
    strcat(str, GPSinfo[indexInGPS[0]]);
    strcat(str, GPSinfo[indexInGPS[1]]);
    
    l.degs = atoi(str);
    
    str[0] = 0;
    
    strcat(str, GPSinfo[indexInGPS[2]]);
    strcat(str, GPSinfo[indexInGPS[3]]);
    
    l.mins = atoi(str);
    
    str[0] = 0;
    
    strcat(str, GPSinfo[indexInGPS[5]]);
    strcat(str, GPSinfo[indexInGPS[6]]);
    strcat(str, GPSinfo[indexInGPS[7]]);
    
    l.secs = atoi(str) * 60;
    
  }
  
  
  
  
  
  
  
  //  int deg, min, sec;
//  char dir;
//
//  // The format of the information is:
//  //   Longitude: dddmm.mmmmm
//  //   Latitude:   ddmm.mmmmm
//  float coord = readFloatUntilComma();
//  /* Read the degrees */
//  deg = (int) (coord/100);
//  
//  /*  Read the minutes/seconds */
//  coord -= deg; 
//  coord *= 100;
//  min = (int) coord;
//  coord -= min;
//  sec = coord*60;
//  // Note: the ',' at the end of the lon/lat has already been consumed
//
//  // Read the latitude direction
//  dir = readChar();
//
//  // Save the parsed results.
//  if (isLongitude) 
//    {
//      lonDeg = deg;
//      lonMin = min;
//      lonSec = sec;
//      lonDir = dir;
//    } 
//  else 
//    {
//      latDeg = deg;
//      latMin = min;
//      latSec = sec;
//      latDir = dir;
//    }

  return true;
}

//float readFloatUntilComma(void) {
//  /* read a floating point number (something of the form of 123.4567,) */
//  char temp;
//  int place=10;
//  float result=0;
//  temp = readChar();
//  
//  while (isDigit(temp))
//  {
//    result = result*10 + (temp-'0');    // Subtracts the ASCII part of the byte - e.g. ASCII '0' will be set to binary 0.
//    temp = readChar();                  // the current result is multiplied by 10 and the latest byte read is added.
//  }
//  
//  if (temp == ',')
//  {
//    return result;
//  }
//  
//  temp = readChar();
//  
//  while (isDigit(temp))
//  {
//    result += (temp-'0')/place;
//    place *= 10;
//    temp = readChar ();
//  }
//  
//  return result;
//}
 
bool isDigit(char c) {
  /* Is the character c within the ascii range 0-9 */
  return (c>='0' && c <='9');
}

// Read in a 'num_chars' integer from serial.
int readInt(int num_chars) {
  return 0;
}
