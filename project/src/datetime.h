#ifndef DATETIME_H
#define DATETIME_H

#include<iostream>
#include<chrono>
#include<string>
#include<ctime>


/**
 * @file datetime.h
 *
 * @brief Declaration of custom Datetime class.
 */

// Custom class for working with dates;
/**
 * @class Datetime
 *
 * @brief Custom class for simplifying datetime conversions.
 */
class Datetime {
    public:
    int year; /**< The year component of the date and time. */
    int month; /**< The month component of the date and time. */
    int day; /**< The day component of the date and time. */
    int hour; /**< The hour component of the date and time. */
    int minute; /**< The minute component of the date and time. */
    int second; /**< The second component of the date and time. */
     /**
     * @brief Default constructor for Datetime.
     */
    Datetime() ;
     /**
     * @brief Constructor for Datetime.
     *
     * @param localTime A pointer to a tm struct representing the local time.
     */
    Datetime(const tm * localTime) ;
    Datetime(const std::string & formatted);
    Datetime(const Datetime & dt);
    Datetime(int y, int m, int d, int h, int mi, int sec);
    Datetime operator=(const Datetime & dt);
    
    ~Datetime() {};
    friend std::ostream & operator << (std::ostream & out, const Datetime & dt);
    /**
     * @brief Converts the Datetime to a string representation.
     *
     * @return The string representation of the Datetime.
     */
    std::string toString() const;
     /**
     * @brief Converts the Datetime to a string representation suitable for file storage.
     *
     * @return The file-friendly string representation of the Datetime.
     */
    std::string toFileString() const;
    /**
     * @brief Overloaded greater than operator for Datetime.
     *
     * @param other The Datetime object to compare against.
     * @return True if the current Datetime is greater than the other Datetime, false otherwise.
     */

    bool operator > (const Datetime & other) const;
    bool operator >= (const Datetime & other) const;

    bool operator <= (const Datetime & other) const;
    bool operator < (const Datetime & other) const;

    bool operator==(const Datetime& other) const;
    


    long long toSeconds() const;

};

#endif // DATETIME_H
