/**
 *  fbTime
 *   Time manager
 *  counts the secounds from midnight
 *  @author Byron Heads
 *  @date March 15, 2008
 */

#include "fbTime.h"

/*
 *  fbTime
 *  Default constructor, initilizes to current local time
 */
fbTime::fbTime():ticks(0), hour(0), min(0), sec(0)
{
	setTicksLocal();
}

/*
 *  fbTime
 *  Constructor, sets time to given values
 *  @param h Hours
 *  @param m Mins
 *  @param s Sounds
 */
fbTime::fbTime(int h, int m, int s):ticks(0), hour(0), min(0), sec(0)
{
	setTicks(h,m,s);
}

/*
 *  fbTime
 *  Constructor, sets time to give tick values, loaded from file?
 *  @param _ticks new tick value
 */
fbTime::fbTime(long _ticks):ticks(_ticks), hour(0), min(0), sec(0)
{
	update();
}

/*
 *  fbTime
 *   constructor, seeds time with tm struct
 *  @param time, tm struct ref
 */
fbTime::fbTime(tm& time):ticks(0), hour(0), min(0), sec(0)
{
	setTicks(time.tm_hour, time.tm_min, time.tm_sec);
}

/*
 *  fbTime
 *   constructor, copies an existing time structures tick value
 *  @param time, fbTime to copy
 */
fbTime::fbTime(fbTime& time):ticks(time.ticks), hour(0), min(0), sec(0)
{
	update();
}

/*
 *  setTicks
 *  Sets the ticks to given time
 *  @param h Hours
 *  @param m Mins
 *  @param s Sounds
 */
void fbTime::setTicks(int h, int m, int s)
{
	ticks = (h * 3600) + (m * 60) + s;
	update();
}

/*
 *  setTicksLocal
 *  Sets the ticks to the current local time
 */
void fbTime::setTicksLocal()
{
	time_t raw = 0;
	time(&raw);
	tm time; // = *localtime(&raw);
#ifndef Win32
	localtime_r(&raw, &time);
#else
	//need to be changed in windows!!
	time = *localtime(&raw);
#endif
	setTicks(time.tm_hour, time.tm_min, time.tm_sec);
}

/*
 *  update
 *  recalculates time based on ticks
 *  call it after anythime ticks is changed!!
 */
void fbTime::update()
{
	ticks %= 86400;
	if (ticks < 0)
		ticks += 86400;

	hour = ticks / 3600;
	sec = ticks % 60;
	min = (ticks - (hour * 3600) - sec) / 60;
}

/*
 *  addMin
 *  Adds the given number of minutes to the time.
 *  If the result crosses midnight, ticks wraps within the day and the
 *  number of whole days that overflowed is returned so the caller can
 *  advance the date accordingly.
 *  @param m  Minutes to add (default 1)
 *  @return   Number of full days carried past midnight (0 if no overflow)
 */
long fbTime::addMin(const int m)
{
	ticks += (long)m * 60;
	long days = ticks / 86400;
	update();   // wraps ticks into [0, 86400)
	return days;
}

/*
 *  addHour
 *  Adds the given number of hours to the time.
 *  If the result crosses midnight, ticks wraps within the day and the
 *  number of whole days that overflowed is returned so the caller can
 *  advance the date accordingly.
 *  @param h  Hours to add (default 1)
 *  @return   Number of full days carried past midnight (0 if no overflow)
 */
long fbTime::addHour(const int h)
{
	ticks += (long)h * 3600;
	long days = ticks / 86400;
	update();   // wraps ticks into [0, 86400)
	return days;
}

void fbTime::hms(string& t)
{
	t += (hour < 10 ? "0" : "");
	ltostr(hour, t);
	t += ":";
	t += (min < 10 ? "0" : "");
	ltostr(min, t);
	t += ":";
	t += (sec < 10 ? "0" : "");
	ltostr(sec, t);
}

void fbTime::hm(string& t)
{
	t += (hour < 10 ? "0" : "");
	ltostr(hour, t);
	t += ":";
	t += (min < 10 ? "0" : "");
	ltostr(min, t);
}

void fbTime::ltostr(long val, string& str)
{
	char buff[20];
	snprintf(buff, sizeof(buff)-1, "%ld", val);
	str += buff;
}

