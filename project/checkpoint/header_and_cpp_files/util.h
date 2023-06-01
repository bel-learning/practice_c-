#ifndef UTIL_H
#define UTIL_H
#include<chrono>
#include "datetime.h"
#include<ctime>
#include<string>


using namespace std;


Datetime getCurrentDateTime();
int dayIndexInWeek(const Datetime & time);
std::string getMonthName (int monthNum);

#endif // UTIL_H
