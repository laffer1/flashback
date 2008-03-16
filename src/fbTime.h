/* $Id: fbTime.h,v 1.2 2008/03/16 01:58:56 wyverex Exp $ */


#ifndef fbTIME_H
#define fbTIME_H

#include "global.h"
#include <ctime>

/**
 *  fbTime
 *   Time manager
 *  counts the secounds from midnight
 *  @author Byron Heads
 *  @date March 15, 2008
 */
class fbTime
{
public:
	fbTime();
	fbTime(int h, int m, int s);
	fbTime(long _ticks);
	fbTime(tm& time);
	fbTime(fbTime& time);

	~fbTime(){};

	//get and set mostly used for saving..
	long getTicks() const {return ticks;};	
	void setTicks(long _ticks) {ticks = _ticks;};

	// set the tick value
	void setTicks(tm& time){setTicks(time.tm_hour, time.tm_min, time.tm_sec);};
	void setTicks(int h, int m, int s);
	void setTicksLocal();
	
	//get the time
	int getHour() const {return hour;};
	int getMin() const {return min;};
	int getSec() const {return sec;}; 

	//comparision operators
	bool operator==(fbTime& rhs) {return ticks == rhs.ticks;};
	bool operator>=(fbTime& rhs) {return ticks >= rhs.ticks;};
	bool operator<=(fbTime& rhs) {return ticks <= rhs.ticks;};
	bool operator>(fbTime& rhs) {return ticks > rhs.ticks;};
	bool operator<(fbTime& rhs) {return ticks < rhs.ticks;};

	//simple add secounds
	fbTime& operator+(const int s){ticks+= s; update(); return *this;};
	fbTime& operator+=(const int s){ticks+= s; update(); return *this;};
	fbTime& operator++(){++ticks; update(); return *this;};
	fbTime& operator++(const int s){++ticks; update(); return *this;};
	
	//remove secs
	fbTime& operator-(const int s){ticks-= s; update(); return *this;};
	fbTime& operator-=(const int s){ticks-= s; update(); return *this;};
	fbTime& operator--(){--ticks; update(); return *this;};
	fbTime& operator--(const int s){--ticks; update(); return *this;};

	void addSec(const int s = 1){*this += s;};
	void addMin(const int m = 1){*this += (m*60);};
	void addHour(const int h = 1){*this += (h*3600);};

private:
	long ticks;
	int hour;
	int min;
	int sec;

	void update();
};

#endif

