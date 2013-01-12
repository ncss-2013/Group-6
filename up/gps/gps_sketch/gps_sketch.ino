#include <SoftwareSerial.h>
#include <GlobalPositioning.h>
#include <encoding.h>

#include <EEPROM.h>

#define RESET_VALUE 0x7FFFFFFF

//#define WAIT_TIME (12*60*1000)
#define WAIT_TIME (10000)
#define PAUSE_TIME (5000)
#define WARNING_TIME (WAIT_TIME - PAUSE_TIME)

#define WARN_BYTE 0x55

#define WRITE_BUFFER_SIZE 10
#define WORD_SIZE 6
#define RESET_PIN 13

GlobalPositioning gps(8, 9, 4800);

//long originLat, originLong;
#define originLat (-121998)
#define originLong (544299)

char writeBuffer[WRITE_BUFFER_SIZE];

void writeEEPROM(long latitude, long longitude);
void readEEPROM(long* latitude, long* longitude);

void setup() {
  Serial.begin(9600); 
  /*
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  
  // Just in case the pull up resistor hasn't done anything yet
  delay(1);
  
  if(digitalRead(RESET_PIN) == LOW) {
    writeEEPROM(RESET_VALUE, RESET_VALUE);
    exit(0);
  }
  
  readEEPROM(&originLat, &originLong);
  if (originLat == RESET_VALUE) {
    delay(PAUSE_TIME); // May receive borked data during this period
    while (gps.readData() != 0)
      ;
    originLat = coordToSeconds(gps.latDeg, gps.latMin,
                               gps.latSec);
    originLong = coordToSeconds(gps.longDeg, gps.longMin,
                                gps.longSec);
  }
  Serial.write(WARN_BYTE);
  packWord(writeBuffer, originLat, originLong, 0xFFFF);
  addChecksum(writeBuffer);
  delay(PAUSE_TIME);
  Serial.write((const uint8_t*)writeBuffer, WORD_SIZE);
  */
}


void loop() {
  unsigned long waitUntil = millis() + WARNING_TIME;
  unsigned long sendAt = millis() + WAIT_TIME;
  while (waitUntil > millis())
    gps.readData();
  Serial.write(WARN_BYTE);
  
  packData(gps.latDeg, gps.latMin, gps.latSec,
           gps.longDeg, gps.longMin, gps.longSec);
  while (sendAt > millis())
    ;
  Serial.write((const uint8_t*)writeBuffer, WORD_SIZE);
}

void packData(int latDeg, int latMin, float latSec,
              int longDeg, int longMin, float longSec) {
  long latitude, longitude, altitude;
  latitude = coordToSeconds(latDeg, latMin, latSec);
  longitude = coordToSeconds(longDeg, longMin, longSec);
  altitude = altitude;
  packWord(writeBuffer, latitude - originLat, longitude - originLong, altitude);
  addChecksum(writeBuffer);
}

void writeEEPROM(long latitude, long longitude)
{
	byte temp[4] = {0,0,0,0};

	// turn latitude into an array of bytes
	temp[0] = latitude >> 12;
	temp[1] = (latitude >> 8) & 0b1111;
	temp[2] = (latitude >> 4) & 0b1111;
	temp[3] = latitude & 15;

	// write latitude to EEPROM
	EEPROM.write(0, temp[0]);
	EEPROM.write(1, temp[1]);
	EEPROM.write(2, temp[2]);
	EEPROM.write(3, temp[3]);

	// turn longitude into an array of bytes
	temp[0] = longitude >> 12;
	temp[1] = (longitude >> 8) & 0b1111;
	temp[2] = (longitude >> 4) & 0b1111;
	temp[3] = longitude & 15;

	// write longitude to EEPROM
	EEPROM.write(4, temp[0]);
	EEPROM.write(5, temp[1]);
	EEPROM.write(6, temp[2]);
	EEPROM.write(7, temp[3]);
}

void readEEPROM(long* latitude, long* longitude)
{
	*latitude = (EEPROM.read(0) * 4096) +
		(EEPROM.read(1) * 256) +
		(EEPROM.read(2) * 16) +
		EEPROM.read(3);

	*longitude = (EEPROM.read(4) * 4096) +
		(EEPROM.read(5) * 256) +
		(EEPROM.read(6) * 16) +
		EEPROM.read(7);
}
