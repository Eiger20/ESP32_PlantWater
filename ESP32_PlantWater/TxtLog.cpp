#include "TxtLog.h"
#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
#include <Arduino.h> 
//#include <iostream>

TxtLog::TxtLog() {
  for(int i = 1; i < dSize; i++) buff[i] = 0;
  next = buff;
}

void TxtLog::incp(char **x) {
  char *y;
  y = *x;
  y++; 
  if(y >= end) y = buff;
  *x = y;
}

char * TxtLog::findNextLine(char *s) {
  while(*s) incp(&s); // stop at terminating null
  incp(&s); // step past terminating null
  return s;
}


void TxtLog::addLine(char *s) {
  while(*s) {
    *next = *s;
    incp(&next);
    s++;
  }
  *next = 0;
  incp(&next);
}



char * TxtLog::dispLine(char *s) {
  // if the line is empty skip it
  if(!*s) {
    incp(&s);
    return s;
  }
  // display line
  while(*s) {
    //std::cout << *s;
    Serial.print(*s);
    incp(&s);
  }
  //std::cout << std::endl;
  Serial.println();
  incp(&s);
  return s;
}

// return true if we are at the end
bool TxtLog::checkForEnd(char *s) {
  if(s == next) return true;
  while(*s) { // stop at terminating null
    incp(&s);
    if(s == next) return true;
  }
  return false;
}

void TxtLog::disp() {
  char *s;
  Serial.println("--- TxtLog::disp() Start ---");
  s = findNextLine(next);
  while(!checkForEnd(s)) {
    s = dispLine(s);
  }
  Serial.println("--- TxtLog::disp() End ---");
}
