#ifndef TxtLog_h
#define TxtLog_h

class TxtLog {
public:
  static const int dSize = 10000;
  char buff[dSize];
  const char *end = buff + dSize;
  char *next;

  TxtLog();
  void incp(char **x);
  char * findNextLine(char *s);
  void addLine(char *s);
  void addLine(const char *s) {addLine((char *) s);}
  char * dispLine(char *s);
  bool checkForEnd(char *s);
  void disp();
};

#endif
