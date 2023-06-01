#include<chrono>
#include "datetime.h"
#include "util.h"
#include<ctime>
#include<string>

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

    return (months[monthNum]); 
}