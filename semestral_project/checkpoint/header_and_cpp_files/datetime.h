#ifndef DATETIME_H
#define DATETIME_H

#include<iostream>
#include<chrono>
#include<ctime>
// Custom class for working with dates;
class Datetime {
    public:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    Datetime() ;
    Datetime(const tm * localTime) ;
    ~Datetime() {};
    friend std::ostream & operator << (std::ostream & out, const Datetime & dt);
    
};

#endif // DATETIME_H
