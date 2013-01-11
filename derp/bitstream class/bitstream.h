#ifndef BITSTREAM
#define BITSTREAM

struct ordinate
{
  unsigned int deg,min, sec;
};

struct message
{
  int latsign : 1;
  int latdeg : 2;
  int latmin : 6;
  int latsec : 16;
  
  int longsign : 1;
  int longdeg : 2;
  int longmin : 6;
  int longsec : 16;
  
  int alt : 16;
  int temp : 6;
  int chksum : 8;
};

class bitStream
{
public:
  void fromBitStream(char *stream);
  void toBitStream(ordinate _lat, ordinate _long, int _alt, int _temp, int _chksum);
  ordinate getLatitude() { return latitude; }
  ordinate getLongitude() { return longitude; }
  int getAltitude() { return altitude; }
  int getTemperature() { return temperature; }
  char getChksum() { return chksum; }
  char *getStream() { return stream; }
private:
  ordinate latitude, longitude;
  int altitude, temperature;
  char chksum;
  char stream[10];
};

#endif //BITSTREAM
