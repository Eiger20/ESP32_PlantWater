#ifndef UtilFunc_h
#define UtilFunc_h

const char * getElapsedTimeString(unsigned long t) {
  static char buf[14];
  int seconds, minutes, hours, days, v;
  const int s = 1000, m = 60, h = 60, d = 24;
  v = s * m * h * d;
  days = t / v;
  t -= days * v;
  v = s * m * h;
  hours = t / v;
  t -= hours * v;
  v = s * m;
  minutes = t / v;
  t -= minutes * v;
  v = s;
  seconds = t / v;
  sprintf(buf, "%02d:%02d:%02d:%02d", days, hours, minutes, seconds);
  return (const char *) buf;
}

#endif