#include "bitstream.h"

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop()
{
  ordinate lat1,long1;
  unsigned int alt1,temp1;
  char chksum;
  alt1 = random(64000+1);
  temp1 = random(64+1);
  lat1.deg = random(4+1)-2;
  lat1.min = random(60+1);
  lat1.sec = random(60000+1);
  long1.deg = random(4+1)-2;
  long1.min = random(60+1);
  long1.sec = random(60000+1);
  chksum = random(255+1);
  
  bitStream bs1,bs2;
  bs1.toBitStream(lat1,long1,alt1,temp1,chksum);
  bs2.fromBitStream(bs1.getStream());
  
  if (!(alt1 == bs2.getAltitude() && temp1 == bs2.getTemperature() && chksum == bs2.getChksum() && lat1.deg == bs2.getLatitude().deg &&
        lat1.min == bs2.getLatitude().min && lat1.sec == bs2.getLatitude().sec && long1.deg == bs2.getLongitude().deg && 
        long1.min == bs2.getLongitude().min && long1.sec == bs2.getLongitude().sec))
  {
  
  Serial.print(alt1);
  Serial.print(" ");
  Serial.println(bs2.getAltitude());
  
  Serial.print(temp1);
  Serial.print(" ");
  Serial.println(bs2.getTemperature());
  
  Serial.print((int)chksum);
  Serial.print(" ");
  Serial.println((int)bs2.getChksum());
  
  Serial.print(lat1.deg);
  Serial.print(" ");
  Serial.print(lat1.min);
  Serial.print(" ");
  Serial.print(lat1.sec);
  Serial.print("   ");
  
  Serial.print(bs2.getLatitude().deg);
  Serial.print(" ");
  Serial.print(bs2.getLatitude().min);
  Serial.print(" ");
  Serial.println(bs2.getLatitude().sec);
  
  //
  Serial.print(long1.deg);
  Serial.print(" ");
  Serial.print(long1.min);
  Serial.print(" ");
  Serial.print(long1.sec);
  Serial.print("   ");
  
  Serial.print(bs2.getLongitude().deg);
  Serial.print(" ");
  Serial.print(bs2.getLongitude().min);
  Serial.print(" ");
  Serial.println(bs2.getLongitude().sec);
  
  for (int i = 0; i < 10; i++)
  {
    //*
    for (int k = 0; k < 8; k++)
    {
      int bitmask = 1<<k;
      int bytei = bs1.getStream()[i];
      Serial.print(!!(bytei&bitmask));
    }
    
    Serial.print(" ");
    /*
    Serial.print((int)bs1.getStream()[i]);
    //*/
  }
  Serial.println("");
  }
  delay(100);
  
}

