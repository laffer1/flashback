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

fbDate::fbDate(long long _julian): julian(_julian), year(0), mon(0), day(0)
{
	update();
}

fbDate::~fbDate(void)
{
}

void fbDate::setJulian(const int _month, const int _day, const int _year)
{
	//http://en.wikipedia.org/wiki/Julian_day
	long long a = (14 - _month) / 12;
	long long y = _year + 4800 - a;
	long long m = _month + (12 * a) - 3;
	julian = _day + (153 * m + 2 ) / 5 + 365 * y + ( y / 4) - (y / 100) + ( y /400) - 32045;
	update();
}

void fbDate::setJulianLocal()
{
	time_t raw(NULL);
	tm time = *localtime(&raw);
	time.tm_year += 1900;
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
	long long j= julian + 32044;
	long long g = j / 146097;
	long long dg = j % 146097;
	long long c = (dg / 36524 + 1) * 3 / 4;
	long long dc = dg - c * 36524;
	long long b = dc / 1461;
	long long db = dc % 1461;
	long long a = (db / 365 +1)* 3 / 4;
	long long da = db - a *365;
	long long y = g * 400 + c * 100 + b * 4 + a;
	long long m = (da * 5 +308) / 153 -2;
	long long d = da - (m+4) * 153 / 5 + 122;
	year = (int)(y - 4800 + (m+2) / 12);
	mon = (int)((m+2) % 12 +1);
	day = (int)((double)d + 1.5);
}




void fbDate::outMDY()
{
	cout << mon << "/" << day << "/" << year << endl;
}
