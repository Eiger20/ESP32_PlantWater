#ifndef SenseLog_h
#define SenseLog_h

#include "UtilFunc.h"

class SenseData {
public:
  uint16_t minute;  
  uint16_t sensor[4];

  void clear() {minute = sensor[0] = sensor[1] = sensor[2] = sensor[3] = 0;}

  void set(uint16_t tminute, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3) {
    minute = tminute; sensor[0] = v0; sensor[1] = v1; sensor[2] = v2; sensor[3] = v3;
  }

  void set(uint16_t tminute, uint16_t dat[]) {
    minute = tminute; sensor[0] = dat[0]; sensor[1] = dat[1]; sensor[2] = dat[2]; sensor[3] = dat[3];
  }

  void get(uint16_t &tminute, uint16_t dat[]) {
    tminute = minute; dat[0] = sensor[0]; dat[1] = sensor[1]; dat[2] = sensor[2]; dat[3] = sensor[3];
  }

  void prnt() {
    unsigned long tt = minute * 60000;
    Serial.print(getElapsedTimeString(tt));
    Serial.print(" ");
    Serial.print(sensor[0]);
    Serial.print(" ");
    Serial.print(sensor[1]);
    Serial.print(" ");
    Serial.print(sensor[2]);
    Serial.print(" ");
    Serial.print(sensor[3]);
    Serial.println();
  }
};

class SenseLog {
public:
  int logDatEntries = 0;
  int logDatIdx = 0;
  static const int logDatIdxCnt = 1000;
  SenseData logDat[logDatIdxCnt];

  SenseLog() {
    for(int i = 0; i < logDatIdxCnt; i++) logDat[i].clear();
  }

  void add(uint16_t tminute, uint16_t dat[]) {
      logDat[logDatIdx].set(tminute, dat);
      logDatIdx++;
      logDatEntries++;
      if(logDatIdx >= logDatIdxCnt) logDatIdx = 0;
  }

  void add(SenseData d) {
      logDat[logDatIdx] = d;
      Serial.print("add(SenseData d) ["); Serial.print(logDatIdx); Serial.print("]  ");
      d.prnt();
      logDatIdx++;
      logDatEntries++;
      if(logDatIdx >= logDatIdxCnt) logDatIdx = 0;
  }

  void dump() {
    int i, n, x;
    Serial.println("------ Sensor Log Start -------------");
    i = logDatIdx - 1;
    if(i < 0) i = logDatIdxCnt - 1;
    x = min(logDatIdxCnt, logDatEntries);
    for(n = 0; n < x; n++) {
      logDat[i].prnt();
      // if(logDat[i].sensor[0]) {
      // }
      i--;
      if(i < 0) i = logDatIdxCnt - 1;
    }
    Serial.println("-------Sensor Log End ---------------");
  }
};

#endif
