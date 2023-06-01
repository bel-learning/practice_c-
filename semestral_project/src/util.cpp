#include<chrono>
#include "datetime.h"
#include "util.h"
#include<ctime>
#include<string>
#include <iostream>
#include <fstream>
Datetime getCurrentDateTime() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm * tmTime = localtime(&currentTime);
    return Datetime(tmTime);
};


int dayIndexInWeek(const Datetime & time) 
{ 
    int year = time.year; int month=time.month ; int day = time.day;

    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 
                    4, 6, 2, 4 }; 
    year -= month < 3; 
    return ( year + year/4 - year/100 + 
            year/400 + t[month-1] + day) % 7; 
} 
std::string getMonthName (int monthNum) {
    std::string months[] = {"Jan", "Feb", "Mar", 
                    "Apr", "May", "Jun", 
                    "Jul", "Aug", "Sep", 
                    "Oct", "Nov", "Dec"
                    }; 

    return (months[monthNum-1]); 
}

int getDaysInMonth(const Datetime & time) {
    int numDays = 31;
    int month = time.month;
    int year = time.year;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        numDays = 30;  // April, June, September, November

    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            numDays = 29;  // February leap year
        else
            numDays = 28;  // February non-leap year
    }
    return numDays;
}   

void writeToDebug(const string & out) {
    ofstream file("../debugger.txt");

    file << out << endl;
    return;
}
void writeToDebug(const char & out) {
    ofstream file("../debugger.txt");

    file << out << endl;
    return;
}
void writeToDebug(const int & out) {
    ofstream file("../debugger.txt");

    file << out << endl;
    return;
}
Datetime increaseMonth(Datetime & date) {
    if(date.month == 12) {
        date.year++;
        date.month = 1;
    }
    else 
        date.month++;
    return date;
}

Datetime decreaseMonth(Datetime & date) {
    if(date.month == 1) {
        date.year--;
        date.month = 12;
    }
    else 
        date.month--;
    return date;
}

Datetime calculateEndtime(const Datetime & start, int durationMinute) {
    long long startInSeconds = start.toSeconds();
    long long durationInSeconds = durationMinute * 60;
    return secondsToDatetime(startInSeconds + durationInSeconds);
}
Datetime secondsToDatetime(long long totalSeconds) {
    Datetime dt;
        
    // Convert seconds to years
    dt.year = totalSeconds / (365 * 24 * 60 * 60);
    totalSeconds %= (365 * 24 * 60 * 60);

    // Convert seconds to months
    dt.month = totalSeconds / (30 * 24 * 60 * 60);
    totalSeconds %= (30 * 24 * 60 * 60);

    // Convert seconds to days
    dt.day = totalSeconds / (24 * 60 * 60);
    totalSeconds %= (24 * 60 * 60);

    // Convert seconds to hours
    dt.hour = totalSeconds / (60 * 60);
    totalSeconds %= (60 * 60);

    // Convert seconds to minutes
    dt.minute = totalSeconds / 60;
    totalSeconds %= 60;

    // Remaining seconds
    dt.second = totalSeconds;

    return dt;

}

Repeat stringToRepeat(const string & formatted) {
    if(formatted == "weekly") return Repeat::Weekly;
    if(formatted == "biweekly") return Repeat::BiWeekly;
    if(formatted == "monthly") return Repeat::Monthly;
    if(formatted == "daily") return Repeat::Daily;
    if(formatted == "yearly") return Repeat::Yearly;
    return Repeat::Yearly;
}
