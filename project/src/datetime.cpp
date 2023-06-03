#include<ncurses.h>
#include<chrono>
#include<ctime>
#include<sstream>
#include "datetime.h"
#include "util.h"
#include<iostream>
#include <fstream>

    


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
Datetime::Datetime(const Datetime & dt) : 
    year(dt.year), month(dt.month), day(dt.day), hour(dt.hour), minute(dt.minute), second(dt.second) {}


Datetime Datetime::operator=(const Datetime & dt) {
    if(this == &dt) return *this;
    year = dt.year;
    month = dt.month;
    day = dt.day;
    hour = dt.hour;
    minute = dt.minute;
    second = dt.second;
    return *this;
}


std::ostream & operator << (std::ostream & out, const Datetime & dt) {
        return out << dt.year << " " << dt.month ;
};
long long Datetime::toSeconds() const {
    long long totalSeconds = 0;

    // Convert years to seconds
    totalSeconds += static_cast<long long>(year) * 365 * 24 * 60 * 60;
    // Convert months to seconds
    totalSeconds += static_cast<long long>(month) * 30 * 24 * 60 * 60;
    // Convert days to seconds
    totalSeconds += static_cast<long long>(day) * 24 * 60 * 60;
    // Convert hours to seconds
    totalSeconds += static_cast<long long>(hour) * 60 * 60;

    // Convert minutes to seconds
    totalSeconds += static_cast<long long>(minute) * 60;
    // Add seconds
    totalSeconds += second;

    return totalSeconds;

}

Datetime::Datetime(const std::string & formatted) {
    // Get current date and time
    Datetime currentTime = getCurrentDateTime();

    // YYYY-MM-DD-hh-mm
    // Counting occurrence to know which format it came in
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
    second = 0;

    int occur = 0;
    for(int i = 0; i < formatted.size(); i++) {
        if(formatted[i] == '-')
            occur++;
    }
    if(occur == 3) {
        std::istringstream iss(formatted);
        std::string token;
        // Parse the date and time components
         std::getline(iss, token, '-');
        if (!token.empty())
            year = std::stoi(token);
   
        std::getline(iss, token, '-');
        if (!token.empty())
            month = std::stoi(token);
        std::getline(iss, token, '-');
        if (!token.empty())
            day = std::stoi(token);

        std::getline(iss, token, '-');
        if (!token.empty())
            hour = std::stoi(token);
    
        std::getline(iss, token, '-');
        if (!token.empty())
            minute = std::stoi(token);
    }
    if(occur == 3) {
        year = currentTime.year;
        
        std::istringstream iss(formatted);
        std::string token;
        // Parse the date and time components
        std::getline(iss, token, '-');
        if (!token.empty())
            month = std::stoi(token);
        std::getline(iss, token, '-');
        if (!token.empty())
            day = std::stoi(token);

        std::getline(iss, token, '-');
        if (!token.empty())
            hour = std::stoi(token);
    
        std::getline(iss, token, '-');
        if (!token.empty())
            minute = std::stoi(token);
    }

    if(occur == 2) {
        year = currentTime.year;
        month = currentTime.month;

        std::istringstream iss(formatted);
        std::string token;
        
        // Parse the date and time components
        std::getline(iss, token, '-');
        if (!token.empty())
            day = std::stoi(token);

        std::getline(iss, token, '-');
        if (!token.empty())
            hour = std::stoi(token);
    
        std::getline(iss, token, '-');
        if (!token.empty())
            minute = std::stoi(token);
    }
    if(occur == 1) {
        year = currentTime.year;
        month = currentTime.month;
        day = currentTime.day;
        std::istringstream iss(formatted);
        std::string token;
        
        // Parse the date and time components
        std::getline(iss, token, '-');
        if (!token.empty())
            hour = std::stoi(token);
    
        std::getline(iss, token, '-');
        if (!token.empty())
            minute = std::stoi(token);
    }

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