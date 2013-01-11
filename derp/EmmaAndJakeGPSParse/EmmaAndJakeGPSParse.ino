#include <SoftwareSerial.h>
#include <string.h>
#include "latlon.h"
#include <stdlib.h>


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
    Serial.print(lon.degs);
    Serial.print(" ");
    Serial.print(lon.mins);
    Serial.print("' ");
    Serial.print(lon.secs);
    Serial.print("\" ");

    Serial.print("Latitude: ");
    Serial.print(lat.degs);
    Serial.print(" ");
    Serial.print(lat.mins);
    Serial.print("' ");
    Serial.print(lat.secs);
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
  parseLongLatitude(false, lat, 0);
  parseLongLatitude(true, lon, 2);



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
  
  for (int i = 0;i<=200;i++)
  {
    str[i] = 0;
  }
  
  
  if (isLongitude)
  {
    appendchar(str, GPSinfo[indexInGPS][0]);
    appendchar(str, GPSinfo[indexInGPS][1]);
    appendchar(str, GPSinfo[indexInGPS][2]);
    
    l.degs = atoi(str);
    
    
   
    
    appendchar(str, GPSinfo[indexInGPS][3]);
    appendchar(str, GPSinfo[indexInGPS][4]);
    
    l.mins = atoi(str);
    
    
    appendchar(str, GPSinfo[indexInGPS][6]);
    appendchar(str, GPSinfo[indexInGPS][7]);
    appendchar(str, GPSinfo[indexInGPS][8]);
    
    Serial.println(str);
    
    l.secs = atoi(str) * 60;
  
  }
  else
  {
    appendchar(str, GPSinfo[indexInGPS][0]);
    appendchar(str, GPSinfo[indexInGPS][1]);
    
    l.degs = atoi(str);
    
    str[0] = 0;
    
    appendchar(str, GPSinfo[indexInGPS][2]);
    appendchar(str, GPSinfo[indexInGPS][3]);
    
    l.mins = atoi(str);
    
    str[0] = 0;
    
    appendchar(str, GPSinfo[indexInGPS][5]);
    appendchar(str, GPSinfo[indexInGPS][6]);
    appendchar(str, GPSinfo[indexInGPS][7]);
    
    l.secs = atoi(str) * 60;
    
  }

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
// 
//bool isDigit(char c) {
//  /* Is the character c within the ascii range 0-9 */
//  return (c>='0' && c <='9');

//}

void appendchar(char * s, char c)
{
   s[strlen(s)] = c;
   s[(strlen(s)+1)] = 0;
}
