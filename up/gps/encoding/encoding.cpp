long coordToSeconds(int deg, int minutes, float seconds) {
  return long(deg)*3600 + long(minutes)*60 + long(seconds + 0.5);
}

void packWord(char* buffer, int latitude, int longitude, int altitude) {
  altitude += 5;
  altitude /= 10;
  buffer[0] = char(latitude>>6);
  buffer[1] = char(latitude<<2);
  buffer[1] |= B11;
  buffer[1] &= B11111100 | longitude>>12;
  buffer[2] = char(longitude>>4);
  buffer[3] = char(longitude<<4);
  buffer[3] |= B1111;
  buffer[3] &= B11110000 | altitude>>8;
  buffer[4] = char(altitude);
}

void addChecksum(char* buffer) {
  int a = 0, b = 0;
  for(int i = 0; i<10; i++) {
    int nibble;
    if(i%2 == 0) {
      nibble = (buffer[i/2]>>4) & B1111;
    } else {
      nibble = buffer[i/2] & B1111;
    }
    a += nibble;
    b += a;
  }
  buffer[5] = char(a<<4);
  buffer[5] |= B1111;
  buffer[5] &= B11110000 | b;
}