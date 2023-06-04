#ifndef UTIL_H
#define UTIL_H
#include<chrono>
#include "datetime.h"
#include<ctime>
#include<string>
#include<iostream>
#include<unordered_map>
#include "event.h"
#include<fstream>

using namespace std;


Datetime getCurrentDateTime();
int dayIndexInWeek(const Datetime & time);
std::string getDayOfWeek(int index);
std::string getMonthName (int monthNum);
int getDaysInMonth(const Datetime & time);
Datetime increaseMonth(Datetime & date);
Datetime decreaseMonth(Datetime & date);

void writeToDebug(const string & out, ofstream & file);
void writeToDebug(const string & out);
void writeToDebug(const int & out);
void writeToDebug(const char & out);
// Duration in mins
Datetime calculateEndtime(const Datetime & start, long long duration) ;
Datetime secondsToDatetime(long long seconds);
Event::Repeat stringToRepeat(const string & formatted);
string repeatToString(const Event::Repeat & repeat);

long long repeatToSeconds(const Event::Repeat & repeat);

Datetime findYesterday(const Datetime& dt);
Datetime findTomorrow(const Datetime& dt);
Datetime startOfNextWeek(const Datetime & dt);
Datetime endOfCurrentWeek(const Datetime & dt);
Datetime startOfPreviosWeek(const Datetime & dt);
Datetime startOfCurrentWeek(const Datetime & dt);


string getInput(WINDOW * win);
string promptInput(WINDOW * win, string question);

#endif // UTIL_H
