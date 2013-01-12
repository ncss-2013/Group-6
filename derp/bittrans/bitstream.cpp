#include "bitstream.h"

void bitStream::fromBitStream(char *stream)
{
  message msg = *(message *)stream;
  latitude.deg = msg.latdeg;
  if (msg.latsign) latitude.deg *= -1;
  latitude.min = msg.latmin;
  latitude.sec = msg.latsec;
  
  longitude.deg = msg.longdeg;
  if (msg.longsign) longitude.deg *= -1;
  longitude.min = msg.longmin;
  longitude.sec = msg.longsec;
  
  altitude = msg.alt;
  temperature = msg.temp;
  chksum = msg.chksum;
}

void bitStream::toBitStream(ordinate _lat, ordinate _long, int _alt, int _temp, char _chksum)
{
  message msg;
  if (_lat.deg < 0)
  {
    msg.latsign = 1;
    _lat.deg *= -1;
  }
  else msg.latsign = 0;
  msg.latdeg = _lat.deg;
  msg.latmin = _lat.min;
  msg.latsec = _lat.sec;
  
  if (_long.deg < 0)
  {
    msg.longsign = 1;
    _long.deg *= -1;
  }
  else msg.longsign = 0;
  msg.longdeg = _long.deg;
  msg.longmin = _long.min;
  msg.longsec = _long.sec;
  
  msg.alt = _alt;
  msg.temp = _temp;
  msg.chksum = _chksum;
  char *charstream = (char*)&msg;
  for (int i = 0; i < NUMBYTES; i++)
  {
    stream[i] = charstream[i];
  }
}
