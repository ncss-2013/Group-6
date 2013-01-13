#include <SoftwareSerial.h>

SoftwareSerial GPSSerial (8,9);

void setup()
{
  Serial.begin(4800);
  
  GPSSerial.begin(4800);
  
}

void loop()
{
  if (GPSSerial.available())
  {
    Serial.write(GPSSerial.read());
    
  }
  
}
