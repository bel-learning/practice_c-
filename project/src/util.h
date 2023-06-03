#ifndef UTIL_H
#define UTIL_H
#include<chrono>
#include "datetime.h"
#include<ctime>
#include<string>
#include<iostream>
#include<unordered_map>
#include "event.h"


using namespace std;


Datetime getCurrentDateTime();
int dayIndexInWeek(const Datetime & time);
std::string getMonthName (int monthNum);
int getDaysInMonth(const Datetime & time);
Datetime increaseMonth(Datetime & date);
Datetime decreaseMonth(Datetime & date);

void writeToDebug(const string & out);
void writeToDebug(const int & out);
void writeToDebug(const char & out);
// Duration in mins
Datetime calculateEndtime(const Datetime & start, int duration) ;
Datetime secondsToDatetime(long long seconds);
Event::Repeat stringToRepeat(const string & formatted);


#endif // UTIL_H
