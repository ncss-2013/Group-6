
#include <GlobalPositioning.h>
#include <SoftwareSerial.h>

GlobalPositioning gps(8,9,4800);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int returnValue = gps.readData();
  if (returnValue == 0)
  {
    Serial.println(returnValue);
    Serial.print(gps.latDeg);
    Serial.print(" degrees ");
    Serial.print(gps.latMin);
    Serial.print("' ");
    Serial.print(gps.latSec);
    Serial.print("\" ");
    Serial.print(gps.latDir);
    Serial.print(" ");
    Serial.print(gps.longDeg);
    Serial.print(" degrees ");
    Serial.print(gps.longMin);
    Serial.print("' ");
    Serial.print(gps.longSec);
    Serial.print("\" ");
    Serial.println(gps.longDir);
    Serial.println(gps.altitude, 5);
  }
}

