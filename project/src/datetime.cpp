#include<ncurses.h>
#include<chrono>
#include<ctime>
#include<sstream>
#include "datetime.h"
#include "util.h"
    


Datetime::Datetime()  {};
Datetime::Datetime(const tm * localTime) {
    this->year = localTime->tm_year + 1900;    // Year since 1900
    this->month = localTime->tm_mon + 1;       // Month (1-12)
    this->day = localTime->tm_mday;            // Day of the month (1-31)
    this->hour = localTime->tm_hour;           // Hour (0-23)
    this->minute = localTime->tm_min;          // Minute (0-59)
    this->second = localTime->tm_sec; 
}
Datetime::Datetime(int y, int m, int d, int h, int mi, int sec) :
    year(y), month(m), day(d), hour(h), minute(mi), second(sec) {}

string Datetime::toString() const {
    std::string str;
    str.append(std::to_string(year) + " ");
    str.append(std::to_string(month)  + " ");
    str.append(std::to_string(day)  + " ");
    str.append(std::to_string(hour)  + " ");
    str.append(std::to_string(minute)  + " ");
    return str;
}


std::ostream & operator << (std::ostream & out, const Datetime & dt) {
        return out << dt.year << " " << dt.month ;
};
long long Datetime::toSeconds() const {
    int totalSeconds = 0;

    // Convert years to seconds
    totalSeconds += year * 365 * 24 * 60 * 60;

    // Convert months to seconds
    totalSeconds += month * 30 * 24 * 60 * 60;

    // Convert days to seconds
    totalSeconds += day * 24 * 60 * 60;

    // Convert hours to seconds
    totalSeconds += hour * 60 * 60;

    // Convert minutes to seconds
    totalSeconds += minute * 60;

    // Add seconds
    totalSeconds += second;

    return totalSeconds;

}

Datetime::Datetime(const std::string & formatted) {
    // Get current date and time
    Datetime currentTime = getCurrentDateTime();
    
    std::istringstream iss(formatted);
    std::string token;
    
    // Parse the date and time components
    std::getline(iss, token, '-');
    if (!token.empty())
        year = std::stoi(token);
    else
        year = currentTime.year; // Use the current year
    
    std::getline(iss, token, '-');
    if (!token.empty())
        month = std::stoi(token);
    else
        month = currentTime.month; // Use the current month
    
    std::getline(iss, token, '-');
    if (!token.empty())
        day = std::stoi(token);
    else
        day = currentTime.day; // Use the current day
    
    std::getline(iss, token, '-');
    if (!token.empty())
        hour = std::stoi(token);
    else
        hour = currentTime.hour; // Use the current hour
    
    std::getline(iss, token, '-');
    if (!token.empty())
        minute = std::stoi(token);
    else
        minute = currentTime.minute; // Use the current minute
    
    second = 0;
}

bool Datetime::operator>(const Datetime& other) const {
    if (year > other.year) {
        return true;
    } else if (year == other.year) {
        if (month > other.month) {
            return true;
        } else if (month == other.month) {
            if (day > other.day) {
                return true;
            } else if (day == other.day) {
                if (hour > other.hour) {
                    return true;
                } else if (hour == other.hour) {
                    if (minute > other.minute) {
                        return true;
                    } else if (minute == other.minute) {
                        if (second > other.second) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
bool Datetime::operator>=(const Datetime& other) const {
    if(*this > other || *this == other) 
        return true;
    return false;
}

bool Datetime::operator<(const Datetime& other) const {
    if(*this > other || *this == other)
        return false;
    return true;
}

bool Datetime::operator<=(const Datetime& other) const {
    if (*this > other)
        return false;
    return true;
}
bool Datetime::operator==(const Datetime& other) const {
    return year == other.year && month == other.month && day == other.day &&
               hour == other.hour && minute == other.minute && second == other.second;
}