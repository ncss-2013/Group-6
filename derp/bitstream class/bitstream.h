#ifndef BITSTREAM
#define BITSTREAM

struct ordinate
{
  int deg, min;
  unsigned int sec;
};

struct message
{
  unsigned int latsign : 1;
  unsigned int latdeg : 2;
  unsigned int latmin : 6;
  unsigned int latsec : 16;
  
  unsigned int longsign : 1;
  unsigned int longdeg : 2;
  unsigned int longmin : 6;
  unsigned int longsec : 16;
  
  unsigned int alt : 16;
  unsigned int temp : 6;
  unsigned int chksum : 8;
};

class bitStream
{
public:
  void fromBitStream(char *stream);
  void toBitStream(ordinate _lat, ordinate _long, int _alt, int _temp, char _chksum);
  ordinate getLatitude() { return latitude; }
  ordinate getLongitude() { return longitude; }
  unsigned int getAltitude() { return altitude; }
  int getTemperature() { return temperature; }
  char getChksum() { return chksum; }
  char *getStream() { return stream; }
  int latdeg;
private:
  ordinate latitude, longitude;
  unsigned altitude, temperature;
  char chksum;
  char stream[10];
};

#endif //BITSTREAM
