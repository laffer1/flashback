/* $Id: fbDate.cpp,v 1.9 2008/04/11 21:42:32 ctubbsii Exp $ */

/**
 *  fbDate
 *  Julian Date Object, used to track backup schedules
 *  @author Byron Heads
 *  @date March 15, 2008
 */


#include "fbDate.h"



fbDate::fbDate(void): julian(0), year(0), mon(0), day(0)
{
	setJulianLocal();
}

fbDate::fbDate(fbDate& date): julian(0), year(0), mon(0), day(0)
{
	julian = date.getJulian();
	update();
}
fbDate::fbDate(tm& time): julian(0), year(0), mon(0), day(0)
{
	setJulian(time.tm_mday, time.tm_mon, time.tm_year);
}

fbDate::fbDate(const int _month, const int _day, const int _year): julian(0), year(0), mon(0), day(0)
{
	setJulian(_month, _day, _year);
}

fbDate::fbDate(unsigned long _julian): julian(_julian), year(0), mon(0), day(0)
{
	update();
}

fbDate::~fbDate(void)
{
}

void fbDate::setJulian(const int _month, const int _day, const int _year)
{
	//http://en.wikipedia.org/wiki/Julian_day
	unsigned long a = (14 - _month) / 12;
	unsigned long y = _year + 4800 - a;
	unsigned long m = _month + (12 * a) - 3;
	julian = _day + (153 * m + 2 ) / 5 + 365 * y + ( y / 4) - (y / 100) + ( y /400) - 32045;
	update();
}

void fbDate::setJulianLocal()
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
	time.tm_year += 1900;
 	time.tm_mon++;
	setJulian(time);
}

int fbDate::getMonth() const
{
	return mon;
}

int fbDate::getDay() const
{
	return day;
}

int fbDate::getYear() const
{
	return year;
}

fbDate& fbDate::operator=(const fbDate& right)
{
	if(this != &right)
	{
		julian = right.julian;
		update();
	}
	return *this;
}


void fbDate::addMonth(const int num)
{
	int y = year;
	int m = mon + (num);

	while(m > 12)
	{
		m -= 12;
		y++;
	}

	setJulian(m,day, y);
}


void fbDate::update()
{
	//http://en.wikipedia.org/wiki/Julian_day
	unsigned long j= julian + 32044;
	unsigned long g = j / 146097;
	unsigned long dg = j % 146097;
	unsigned long c = (dg / 36524 + 1) * 3 / 4;
	unsigned long dc = dg - c * 36524;
	unsigned long b = dc / 1461;
	unsigned long db = dc % 1461;
	unsigned long a = (db / 365 +1)* 3 / 4;
	unsigned long da = db - a *365;
	unsigned long y = g * 400 + c * 100 + b * 4 + a;
	unsigned long m = (da * 5 +308) / 153 -2;
	unsigned long d = da - (m+4) * 153 / 5 + 122;
	year = (int)(y - 4800 + (m+2) / 12);
	mon = (int)((m+2) % 12 +1);
	day = (int)((double)d + 1.5);
}

void fbDate::mdy(string& d)
{
	d += (mon < 10 ? "0" : "");
	ltostr(mon, d);
	d += "/";
	d += (day < 10 ? "0" : "");
	ltostr(day, d);
	d += "/";
	ltostr(year, d);
}

void fbDate::my(string& d)
{
	d += (mon < 10 ? "0" : "");
	ltostr(mon, d);
	d += "/";
	ltostr(year, d);
}

void fbDate::ltostr(long val, string& str)
{
	char buff[20];
	snprintf(buff, sizeof(buff)-1, "%ld", val);
	str += buff;
}

