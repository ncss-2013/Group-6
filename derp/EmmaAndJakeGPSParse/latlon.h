struct latlon {
  latlon(int d, int m, int s) : degs(d), mins(m), secs(s) {
  }
  int degs;
  unsigned int mins;
  unsigned int secs;
};
