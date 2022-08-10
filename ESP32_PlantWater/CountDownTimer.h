/*
All values are in milliseconds (1000 = 1 second).
Use set() if there is a possible lag longer than the count down timer interval such as 
a long setup period.

expired(): test if the count down has finished
reset(): reset the count down timer for the next interval as if it never stopped.
		reset() should only be used after expired().
*/
#ifndef CountDownTimer_h
#define CountDownTimer_h

class CountDownTimer {
public:
	long cnt;
private:
	long intervl;
	long hc;

public:
	CountDownTimer() {set(32000);} // just a really high number for default constructor
	CountDownTimer(long intervl) {set(intervl);}
	inline void set(long newInterval) {intervl = newInterval; hc = cnt = millis() + intervl;}
	inline long getInterval() {return intervl;}
	inline long get() {cnt = hc - millis(); return cnt;} // get the current value of count down
	inline boolean expired() {get(); return cnt <= 0;}	// return true if count down has expired
	inline void reset() {hc = intervl + hc; get();}	// resets the count down; use only after expired()
	inline void restart() {set(intervl);} // restart countdown using previous interval
};

#endif
