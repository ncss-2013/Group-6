#include <SoftwareSerial.h>
#include <string.h>
#include "latlon.h"
#include <stdlib.h>


// Declare a software serial port to interface with the GPS unit.
// The receive and transmit pins are 8 and 9 respectively.
SoftwareSerial GPSSerial(8, 9); // RX, TX

latlon lat(0,0,0);
latlon lon(0,0,0);

int altitude;

bool rmc;
bool gga;

//Container for split GPS data strings
char * GPSinfo[200];

char * GGAinfo[200];

 
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
  if (readGPSData() && rmc) {
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
    Serial.println("\" ");
  }
  
  if (readGPSData() && gga)
  {
   Serial.print("Altitude: ");
   Serial.println(altitude); 
  }
}

//works out what type of GPS data it is
bool readGPSData()
{
  char sent[2];
  wipeArray(sent,2);
  
  // Read a byte from the GPS receiver
  if (readChar() != '$') {
    return false;
  }
  
  // Ignore the first two bytes which are always "GP".
  if (!(readChar() == 'G' && readChar() == 'P')) {
  	  return false;
  }
  // Read the next three bytes which specify the header
 
 for (int i = 0;i<3;i++)
  {
   appendchar(sent,readChar());
  }
 
 if (strcmp(sent,"RMC")==0)
 {
  Serial.println("Found RMC sentence");
  rmc = rmcParse(); // Parses the RMC data to find latitude and longitude
 }
 else if (strcmp(sent,"GGA")==0)
 {
    //Serial.println("Found GGA sentence");
    //gga = ggaParse();
 }
 else
 {
    return false;
 }
 return true;
}




bool rmcParse()
{
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

//  for (int i=0; i<200;i++)
//  wipeArray(GPSinfo[i],strlen(GPSinfo[i]));
  
  Serial.println("z ");
  splitByComma(GPSinfo);
  
  Serial.println("a ");
  /* Parse Latitude and Longitude */
  parseLongLatitude(false, lat, 0);
  parseLongLatitude(true, lon, 2);
  Serial.println("b ");

  return true;
}



bool ggaParse()
{
  if (readChar() != ',')
  {
    return false;
  }
  
 
 splitByComma(GGAinfo);
 

// Checks if there is a GGA GPS fix - will drop out if fix is 0

 if (GGAinfo[5] == 0)
 {
   Serial.println("No GPS fix");
   return false;
 }
 
  altitude = parseAlt(GGAinfo[7]);
  return true;
}


int parseAlt(char * str)
{
  char temp[10];
  int i = 0;
  int length = strlen(str);
  
  for (i=0;i<length;i++)
  {
    if (str[i] == '.')
    {
      break;
    }
    
    appendchar(temp,str[i]);
  }
  return atoi(temp);
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
bool parseLongLatitude(bool isLongitude, latlon &l, int indexInGPS)
{
  char str[200];
  
  wipeArray(str,200);
  
  if (isLongitude)
  {
    appendchar(str, GPSinfo[indexInGPS][0]);
    appendchar(str, GPSinfo[indexInGPS][1]);
    appendchar(str, GPSinfo[indexInGPS][2]);
    
    l.degs = atoi(str);
    
    wipeArray(str,200);
   
    
    appendchar(str, GPSinfo[indexInGPS][3]);
    appendchar(str, GPSinfo[indexInGPS][4]);
    
    l.mins = atoi(str);
    wipeArray(str,200);
  
    appendchar(str, GPSinfo[indexInGPS][6]);

    appendchar(str, GPSinfo[indexInGPS][7]);
   
    appendchar(str, GPSinfo[indexInGPS][8]);
    
    l.secs = atoi(str) * 60;
  
  }
  else
  {
    
    appendchar(str, GPSinfo[indexInGPS][0]);
    appendchar(str, GPSinfo[indexInGPS][1]);
   
    l.degs = atoi(str);
    
    
    wipeArray(str,200);
    
    appendchar(str, GPSinfo[indexInGPS][2]);
    appendchar(str, GPSinfo[indexInGPS][3]);
    

    l.mins = atoi(str);
    
    wipeArray(str,200);
    
    appendchar(str, GPSinfo[indexInGPS][5]);
    appendchar(str, GPSinfo[indexInGPS][6]);
    appendchar(str, GPSinfo[indexInGPS][7]);
    


    l.secs = atoi(str) * 60;
    
  }

  return true;
}


void appendchar(char * s, char c)
{
   s[strlen(s)] = c;
   s[(strlen(s)+1)] = 0;
}

void wipeArray(char * array, int length)
{
  for (int i = 0; i < length; i++)
  {
    array[i] = 0;
  }
}

void splitByComma(char * array[200]) 
{

//in parameter array we have raw unsplit GPS data
  char line[1500];
  int length = GPSSerial.readBytesUntil('\r', line,100);
  line[length] = 0;


  
  Serial.println(line);

  
  char * pch;

  int i = 0;
  
  // First call as required, just initializes the split   
  pch = strtok( line, "," );
   
  while (pch != NULL)
  {
    //Serial.println(pch);
    array[i++] = pch;
    pch = strtok (NULL, ",");
    

  }
}
