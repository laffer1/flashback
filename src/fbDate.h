#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iostream>
using namespace std;

class fbDate
{
public:
	fbDate(void);
	fbDate(fbDate& date);
	fbDate(tm& time);
	fbDate(const int _month, const int _day, const int _year);
	fbDate(long long _julian);

	~fbDate(void);

	long long getJulian() const {return julian;};
	void setJulian(const long long _julian) {julian = _julian; update();};
	void setJulian(const int _month, const int _day, const int _year);
	void setJulian(tm& time){setJulian(time.tm_mon, time.tm_mday, time.tm_year);};
	void setJulianLocal();

	int getMonth() const;
	int getDay() const;
	int getYear() const;


	fbDate& operator=(const fbDate& right);
	
	//add day
	fbDate& operator+(const int right){julian+= right; update();return *this;};
	fbDate& operator+=(const int right){julian+= right; update();return *this;};
	fbDate& operator++(){++julian; update();return *this;};
	fbDate& operator++(const int right){++julian; update();return *this;};

	//subtract day
	fbDate& operator-(const int right){julian-= right; update();return *this;};
	fbDate& operator-=(const int right){julian-= right; update();return *this;};
	fbDate& operator--(){--julian; update();return *this;};
	fbDate& operator--(const int right){--julian; update();return *this;};

	void addDay(const int num = 1){*this += num;};
	void addWeek(const int num = 1){*this += 7 * num;};
	void addMonth(const int num = 1);
	


	//output
	void outMDY();


private:
	long long julian;
	int year;
	int mon;
	int day;



	void update();

};
