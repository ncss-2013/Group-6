#include <SoftwareSerial.h>
#include <string.h>
#include "latlon.h"
#include <stdlib.h>
#include "bitstream.h"

#define to10Secs(d,m,s) ((s)+(m*60*10)+(d*60*60*10))

long startLat = to10Secs(33,0,0);
long startLon = to10Secs(151,0,0);

#define COMMUNICATE (13)
#define DELAY (1000/40)

// Declare a software serial port to interface with the GPS unit.
// The receive and transmit pins are 8 and 9 respectively.
SoftwareSerial GPSSerial(8, 9); // RX, TX
bitStream bs;

latlon lat(0,0,0);
latlon lon(0,0,0);

char line[100];

int altitude;

bool rmc;
bool gga;

char * converteddatastream;

//Container for split GPS data strings
char * GPSinfo[20];

 
void setup()
{
  // Initialise hardware serial port for communication with the PC
  Serial.begin(9600);

  // Initialise software serial port for communication with the GPS
  // The default speed on the GPS is 4800 baud.
  GPSSerial.begin(4800);
  pinMode(COMMUNICATE, OUTPUT);
}

void loop()
{
  digitalWrite(COMMUNICATE, LOW); 
  bool data = readGPSData();
  if (data && rmc) {
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
  
  if (data && gga)
  {
   Serial.print("Altitude: ");
   Serial.println(altitude); 
  }
  
  if (data && (rmc && gga))
  {
    Serial.println("Sending...");
    long lat2 = to10Secs(lat.degs,lat.mins,lat.secs);

    long lon2 = to10Secs(lon.degs,lon.mins,lon.secs);
    
    Serial.println(lat2);
    Serial.println(lon2);

    bs.toBitStream(startLat-lat2, startLon-lon2, altitude, 0, 0B10101010);
    converteddatastream = bs.getStream();

    long time = millis();
    digitalWrite(COMMUNICATE,HIGH);
  

  for (int i = 0; i < 8; i++)
  {
    //*
    for (int k = 0; k < 8; k++)
    {
      int bitmask = 1<<k;
      int bytei = converteddatastream[i];
      bool bit = !!(bytei&bitmask);
      while (millis()-time < DELAY);
      digitalWrite(COMMUNICATE,bit);
      time = millis();
      }
    }
  }
}

//works out what type of GPS data it is
bool readGPSData()
{
  char sent[10];
  wipeArray(sent, sizeof(sent) / sizeof(char));
  
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
    Serial.println("Found GGA sentence");
    gga = ggaParse();
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
  if (readChar() != 'A')
  {
    Serial.println("Fell out without active GPS");
    return false;
  }

  if (readChar() != ',')
  {
    return false;
  }


  
  splitByComma();
  
  /* Parse Latitude and Longitude */
  parseLongLatitude(false, lat, 0);
  parseLongLatitude(true, lon, 2);


  return true;
}



bool ggaParse()
{
  if (readChar() != ',')
  {
    return false;
  }
  
 Serial.println("about to split");
 splitByComma();
 Serial.println("finished split");

// Checks if there is a GGA GPS fix - will drop out if fix is 0

 if (atoi(GPSinfo[5]) == 0)
 {
   Serial.println("No GPS fix");
   return false;
 }
 Serial.println("about to parse a;t");
 altitude = parseAlt(GPSinfo[8]);
 Serial.println("finished parse");
  
  return true;
}


int parseAlt(char * str)
{
  char temp[10];
  for (int i = 0; i < 10; i++) temp[i] = 0; 
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
  temp[i] = '\0';

  return atoi(temp);
}



// Define a function to read a byte from the GPS receiver
char readChar()
{
 // Serial.print("startread '");
  int c;
  while ((c = GPSSerial.read()) == -1)
    /* Do Nothing */;
  Serial.write(c);
 // Serial.println("' endread");
  return c;
}

// Define the function to read the long/lat
bool parseLongLatitude(bool isLongitude, latlon &l, int indexInGPS)
{
  char str[30];
  
  wipeArray(str,30);


  
  if (isLongitude)
  {
    appendchar(str, GPSinfo[indexInGPS][0]);
    appendchar(str, GPSinfo[indexInGPS][1]);
    appendchar(str, GPSinfo[indexInGPS][2]);
    
    l.degs = atoi(str);
    
    wipeArray(str,10);
   
    
    appendchar(str, GPSinfo[indexInGPS][3]);
    appendchar(str, GPSinfo[indexInGPS][4]);
    
    l.mins = atoi(str);
    wipeArray(str,10);
  
    appendchar(str, GPSinfo[indexInGPS][6]);

    appendchar(str, GPSinfo[indexInGPS][7]);
   
    appendchar(str, GPSinfo[indexInGPS][8]);
    
    l.secs = atoi(str) * 60/100;
  
  }
  else
  {
    
    appendchar(str, GPSinfo[indexInGPS][0]);
    appendchar(str, GPSinfo[indexInGPS][1]);
   
    l.degs = atoi(str);
    
    
    wipeArray(str,10);
    
    appendchar(str, GPSinfo[indexInGPS][2]);
    appendchar(str, GPSinfo[indexInGPS][3]);
    

    l.mins = atoi(str);
    
    wipeArray(str,10);
    
    appendchar(str, GPSinfo[indexInGPS][5]);
    appendchar(str, GPSinfo[indexInGPS][6]);
    appendchar(str, GPSinfo[indexInGPS][7]);
    


    l.secs = atoi(str) * 60;
    
  }

  return true;
}


void appendchar(char * s, char c)
{
   int l = strlen(s);
   s[l] = c;
   s[l+1] = 0;
}

void wipeArray(char * array, int length)
{
  for (int i = 0; i < length; i++)
  {
    array[i] = 0;
  }
}

void splitByComma() 
{

//in parameter array we have raw unsplit GPS data
  int length = GPSSerial.readBytesUntil('\r', line, 100);
  line[length] = 0;


  char * pch;

  int i = 0;
  
  // First call as required, just initializes the split   
  pch = strtok( line, "," );
   
  while (pch != NULL)
  {

    GPSinfo[i++] = pch;
    pch = strtok (NULL, ",");
  }
}
