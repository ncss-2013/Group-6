#ifndef BITSTREAM
#define BITSTREAM

#define NUMBYTES 8

struct ordinate
{
  int deg, min;
  unsigned int sec;
};

struct message
{
  unsigned long lat : 19;
  unsigned long lon : 19;
  /*
  unsigned int latsign : 1;
  unsigned int latdeg : 2;
  unsigned int latmin : 6;
  unsigned int latsec : 10;//16;
  
  unsigned int longsign : 1;
  unsigned int longdeg : 2;
  unsigned int longmin : 6;
  unsigned int longsec : 10;//16;
  */
  unsigned long alt : 16;
  unsigned long temp : 6;
  unsigned long chksum : 4;//8;
};

class bitStream
{
public:
  void fromBitStream(char *stream);
  void toBitStream(long _lat, long _long, int _alt, int _temp, char _chksum);
  long getLatitude() { return latitude; }
  long getLongitude() { return longitude; }
  unsigned int getAltitude() { return altitude; }
  int getTemperature() { return temperature; }
  char getChksum() { return chksum; }
  char *getStream() { return stream; }
  int latdeg;
private:
  long latitude, longitude;
  unsigned altitude, temperature;
  char chksum;
  char stream[NUMBYTES];
};

#endif //BITSTREAM
