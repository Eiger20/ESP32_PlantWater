#include "CountDownTimer.h"
#include "TxtLog.h"
#include "SenseLog.h"
#include "PumpControl.h"
#include "UtilFunc.h"

#define LED_BUILTIN 2
/*
Test of the moister sensors:
Open air: 2480  Tap water: 1960  Moist soil: 1600  Very dry soil: 2480  Dry soil: 2220
Not connected: 0

*/
CountDownTimer timeLED(500);
CountDownTimer timeMoistureCheck(1000 * 60 * 10);  // Moisture check every 10 minutes.
const long waterPumpTime = 15000;  // pump water for 15 seconds
CountDownTimer timeWaterPumpOn(waterPumpTime);

SenseLog waterLog;

int ledState = 0;
int ledPin = LED_BUILTIN;
TxtLog tl;

const int sensePin0 = 33;
const int sensePin1 = 32;
const int sensePin2 = 35;
const int sensePin3 = 34;
int sensePin[] = {sensePin0, sensePin1, sensePin2, sensePin3};
uint16_t senseAry[4];

const int pumpPin0 = 23;
const int pumpPin1 = 22;
const int pumpPin2 = 17;
const int pumpPin3 = 16;

// Parameters: 
// pump pin numbers 0 thru 3
// pump sensor threshold 0 thru 3
PumpControl pumpCtrl(pumpPin0, pumpPin1, pumpPin2, pumpPin3, 
                      2200, 1700, 2200, 1800);

uint16_t cnt = 1;
SenseData senseDat;

unsigned long mTime;
char buffer[80];

inline void blinkLED() {
  if(timeLED.expired()) {
    timeLED.reset();
    ledState = ~ledState;
    digitalWrite(ledPin, ledState);
  }
}


inline void logDumpRequestCheck() {
  if (Serial.available() > 0) {
    // get incoming byte:
    int inByte = Serial.read();
    if(inByte == 'D' || inByte == 'd') {
      waterLog.dump();
      tl.disp();
    }
    // flush remaining incoming serial data
    while(Serial.available() > 0) Serial.read(); 
  }    
}

void readSensors(int pinAry[], uint16_t avgAry[], int sz) {
  int i, n, k, maxV, minV, totV;
  // esp32 analog input is a bit erratic. We'll smooth is out by taking
  // multiple samples. Take 12 samples throwing out the highest and lowest
  // values. Average the rest.
  Serial.println("--- readSensors() ---");
  int rv[4][16];
  for(i = 0; i < 12; i++) {
    for(n = 0; n < sz; n++) {
      rv[n][i] = analogRead(pinAry[n]);
      //rv[n][i] = random(1550, 2480);
      delay(5);
    }
  }
  for(n = 0; n < sz; n++) {
    maxV = 0;
    minV = 32000;
    totV = 0;     
    for(i = 0; i < 12; i++) {
      maxV = max(maxV, rv[n][i]);
      minV = min(minV, rv[n][i]);
    }
    for(i = 0; i < 12; i++) {
      if(rv[n][i] == maxV) {
        rv[n][i] = 0;
        maxV = -1;
      }
      if(rv[n][i] == minV) {
        rv[n][i] = 0;
        minV = -1;
      }
      totV += rv[n][i]; 
    }
    totV /= 10;
    avgAry[n] = (uint16_t) totV;
  }
  sprintf(buffer, "%s  a0: %d  a1: %d  a2: %d  a3: %d", 
    getElapsedTimeString(mTime), avgAry[0], avgAry[1], avgAry[2], avgAry[3]);
  Serial.println(buffer);
}


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  delay(5000);
  Serial.println("starting 2");
  delay(2000);
  Serial.println("starting 1");
  delay(2000);
  Serial.println("starting 0");
  delay(2000);

  pinMode(sensePin0, INPUT);
  pinMode(sensePin1, INPUT);
  pinMode(sensePin2, INPUT);
  pinMode(sensePin3, INPUT);
  pinMode(pumpPin0, OUTPUT);
  pinMode(pumpPin1, OUTPUT);
  pinMode(pumpPin2, OUTPUT);
  pinMode(pumpPin3, OUTPUT);
  digitalWrite(pumpPin0, LOW);
  digitalWrite(pumpPin1, LOW);
  digitalWrite(pumpPin2, LOW);
  digitalWrite(pumpPin3, LOW);
  // prime the analog input pins
  readSensors(sensePin, senseAry, 4);
}


void loop() {
  blinkLED();

  // If a water pump has been on long enough turn it off.
  if(pumpCtrl.pumping()) {
    if(timeWaterPumpOn.expired()) { // time to turn water pump off?
      mTime = millis();
      int offPin = pumpCtrl.setPumpOffPin();
      digitalWrite(offPin, LOW);
      sprintf(buffer, "%s watering ends", getElapsedTimeString(mTime));
      tl.addLine(buffer);
      Serial.println(buffer);
    }
  }


  if(timeMoistureCheck.expired()) {
    timeMoistureCheck.reset();
    // Get sensor data.
    mTime = millis();
    //Serial.print(mTime); Serial.print(" "); Serial.println(cnt);
    //cnt++;
    readSensors(sensePin, senseAry, 4);
    senseDat.set((uint16_t) (mTime / 60000), senseAry);
    waterLog.add(senseDat);
    // Pump control. Turn on only one pump at a time. -1 = no pump on.
    // If no pumps are on. Check to turn one on.
    if(!pumpCtrl.pumping()) {
      int pin = pumpCtrl.setPumpOnPin(senseDat);
      //Serial.print("pumping pin A: "); Serial.println(pin);
      if(pin >= 0) {
        //Serial.print("pumping pin B: "); Serial.println(pin);
        timeWaterPumpOn.set(waterPumpTime);
        digitalWrite(pin, HIGH);
        sprintf(buffer, "%s watering begins pump %d", getElapsedTimeString(mTime), pumpCtrl.getPumpOnNum());
        tl.addLine(buffer);
        Serial.println(buffer);
      }
    }
  }

  logDumpRequestCheck();
}
