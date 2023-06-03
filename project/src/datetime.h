#ifndef DATETIME_H
#define DATETIME_H

#include<iostream>
#include<chrono>
#include<string>
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
    Datetime(const std::string & formatted);
    Datetime(const Datetime & dt);
    Datetime(int y, int m, int d, int h, int mi, int sec);
    Datetime operator=(const Datetime & dt);
    ~Datetime() {};
    friend std::ostream & operator << (std::ostream & out, const Datetime & dt);
    std::string toString() const;
    bool operator > (const Datetime & other) const;
    bool operator >= (const Datetime & other) const;

    bool operator <= (const Datetime & other) const;
    bool operator < (const Datetime & other) const;

    bool operator==(const Datetime& other) const;
    


    long long toSeconds() const;

};

#endif // DATETIME_H
