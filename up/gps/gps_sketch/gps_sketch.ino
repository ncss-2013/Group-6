#include <SoftwareSerial.h>
#include <GlobalPositioning.h>
#include <encoding.h>

//#define WAIT_TIME (12*60*1000)
#define WAIT_TIME (10000)
#define WARNING_TIME (WAIT_TIME - 5*1000)

#define WARN_BYTE 0x55

#define WRITE_BUFFER_SIZE 10
#define WORD_SIZE 6

GlobalPositioning gps(8, 9, 4800);

long originLat, originLong;

char writeBuffer[WRITE_BUFFER_SIZE];

void setup() {
  Serial.begin(9600); 
  while (gps.readData() != 0)
    ;
  originLat = coordToSeconds(gps.latDeg, gps.latMin,
                             gps.latSec);
  originLong = coordToSeconds(gps.longDeg, gps.longMin,
                              gps.longSec);
}

void loop() {
  unsigned long waitUntil = millis() + WARNING_TIME;
  unsigned long sendAt = millis() + WAIT_TIME;
  while (waitUntil > millis()) 
    gps.readData();
  Serial.write(WARN_BYTE);
  
  long latitude, longitude, altitude;
  latitude = coordToSeconds(gps.latDeg, gps.latMin,
                            gps.latSec);
  longitude = coordToSeconds(gps.longDeg, gps.longMin,
                             gps.longSec);
  altitude = altitude;
  packWord(writeBuffer, latitude - originLat, longitude - originLong, altitude);
  addChecksum(writeBuffer);
  while (sendAt > millis())
    ;
  Serial.write((const uint8_t*)writeBuffer, WORD_SIZE);
}
