#include<ncurses.h>
#include<chrono>
#include<ctime>
#include "datetime.h"
    
Datetime::Datetime()  {};
Datetime::Datetime(const tm * localTime) {
    this->year = localTime->tm_year + 1900;    // Year since 1900
    this->month = localTime->tm_mon + 1;       // Month (0-11)
    this->day = localTime->tm_mday;            // Day of the month (1-31)
    this->hour = localTime->tm_hour;           // Hour (0-23)
    this->minute = localTime->tm_min;          // Minute (0-59)
    this->second = localTime->tm_sec; 
}
std::ostream & operator << (std::ostream & out, const Datetime & dt) {
        return out << dt.year << " " << dt.month ;
};