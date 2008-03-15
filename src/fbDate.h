/* $Id: fbDate.h,v 1.3 2008/03/15 21:18:32 wyverex Exp $ */

#ifndef fbDATE_H
#define fbDATE_H

#include <ctime>
#include "global.h"

/**
 *  fbDate
 *  Julian Date Object, used to track backup schedules
 *  @author Byron Heads
 *  @date March 15, 2008
 */
class fbDate
{
public:

	fbDate(void);  /// < seeds with localtime
	fbDate(fbDate& date);  /// < copies julian
	fbDate(tm& time);      /// < convert from tm structure
	fbDate(const int _month, const int _day, const int _year);   /// < manual input
	fbDate(long long _julian);   /// < julian values... from file anyone?

	~fbDate(void);  /// < clean up.. not sure we have anything here yet?

	long long getJulian() const {return julian;};  /// < get the julian value, for saving?

	void setJulian(const long long _julian) {julian = _julian; update();};  /// < set from julian
	void setJulian(const int _month, const int _day, const int _year);  /// < set from manule date
	void setJulian(tm& time){setJulian(time.tm_mon, time.tm_mday, time.tm_year);};  /// < set from tm struct
	void setJulianLocal();  /// < sets to local date

	int getMonth() const;  /// < get the month value
	int getDay() const;    /// < get the day value
	int getYear() const;   /// < get the year value


	fbDate& operator=(const fbDate& right);  /// < assignment operator
	
	/// add day to julian
	fbDate& operator+(const int right){julian+= right; update();return *this;};
	fbDate& operator+=(const int right){julian+= right; update();return *this;};
	fbDate& operator++(){++julian; update();return *this;};
	fbDate& operator++(const int right){++julian; update();return *this;};

	//subtract day from julian
	fbDate& operator-(const int right){julian-= right; update();return *this;};
	fbDate& operator-=(const int right){julian-= right; update();return *this;};
	fbDate& operator--(){--julian; update();return *this;};
	fbDate& operator--(const int right){--julian; update();return *this;};

	void addDay(const int num = 1){*this += num;};   /// < add a day or more
	void addWeek(const int num = 1){*this += 7 * num;};  /// < add a week or more
	void addMonth(const int num = 1);   /// < add a month or more
	
	//compare julian dates
	bool operator==(const fbDate& right) {return julian == right.julian;};
	bool operator>=(const fbDate& right) {return julian >= right.julian;};
	bool operator<=(const fbDate& right) {return julian <= right.julian;};
	bool operator>(const fbDate& right) {return julian > right.julian;};
	bool operator<(const fbDate& right) {return julian < right.julian;};


	//output --- !!!!! needs to be added!
	void outMDY();  //old debugger test, dont use! maybe remove this before submit


private:
	long long julian;	/// < the julian day
	int year;		/// < the date year
	int mon; 		/// < the date month
	int day;		/// < the date's day
		
	void update();		/// < call update when the julian changes to update year, mon, day
				/// its a long calculation, so don't call to often
};

#endif
