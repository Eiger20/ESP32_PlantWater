#ifndef PumpControl_h
#define PumpControl_h

#include "SenseLog.h"

class PumpControl {
public:
  static const int numOfPumps = 4;
  int pumpOnNum = -1;
  int pin[numOfPumps];
  int onValue[numOfPumps];

  PumpControl(int pumpPin0, int pumpPin1, int pumpPin2, int pumpPin3, 
          int onValue0, int onValue1, int onValue2, int onValue3) {
    pin[0] = pumpPin0; pin[1] = pumpPin1; pin[2] = pumpPin2; pin[3] = pumpPin3;
    onValue[0] = onValue0; onValue[1] = onValue1; onValue[2] = onValue2; onValue[3] = onValue3;
  }

  // saves and returns the pin of the pump to turn on
  int setPumpOnPin(SenseData d) {
    pumpOnNum = -1;
    for(int i = 0; i < numOfPumps; i++) {
      if(d.sensor[i] >= onValue[i]) pumpOnNum = i;
    }
    if(pumpOnNum == -1) return pumpOnNum;
    return pin[pumpOnNum];
  }

  // saves and returns the pin of the pump to turn off
  int setPumpOffPin() {
    int rtn;
    if(pumpOnNum >= 0) rtn = pin[pumpOnNum];
    pumpOnNum = -1;
    return rtn;
  }

  // returns -1 if no pump is on
  int getPumpOnNum() {return pumpOnNum;}

  bool pumping() {
    if(pumpOnNum < 0) return false;
    return true; 
  }
};

#endif PumpControl_h
