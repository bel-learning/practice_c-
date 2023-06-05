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

/**
 * @file util.h
 *
 * @brief Declaration of Helper functions commonly used in the program.
 */

/**
 * @brief Get the current date and time.
 *
 * @return The current date and time as a Datetime object.
 */
Datetime getCurrentDateTime();

/**
 * @brief Get the index of the day in the week for a given date and time.
 *
 * @param time The date and time.
 * @return The index of the day in the week (0-6, starting from Sunday).
 */
int dayIndexInWeek(const Datetime & time);

/**
 * @brief Get the name of the day of the week for a given index.
 *
 * @param index The index of the day in the week (0-6, starting from Sunday).
 * @return The name of the day of the week.
 */
std::string getDayOfWeek(int index);

/**
 * @brief Get the name of the month for a given month number.
 *
 * @param monthNum The month number (1-12).
 * @return The name of the month.
 */
std::string getMonthName(int monthNum);

/**
 * @brief Get the number of days in a month for a given date and time.
 *
 * @param time The date and time.
 * @return The number of days in the month.
 */
int getDaysInMonth(const Datetime & time);

/**
 * @brief Increase the month component of a given date and time by one.
 *
 * @param date The date and time to be increased.
 * @return The resulting date and time.
 */
Datetime increaseMonth(Datetime & date);

/**
 * @brief Decrease the month component of a given date and time by one.
 *
 * @param date The date and time to be decreased.
 * @return The resulting date and time.
 */
Datetime decreaseMonth(Datetime & date);

/**
 * @brief Write a string to the debug output file.
 *
 * @param out The string to write to the debug output file.
 * @param file The output file stream.
 */
void writeToDebug(const string & out, ofstream & file);

/**
 * @brief Write a string to the debug output file.
 *
 * @param out The string to write to the debug output file.
 */
void writeToDebug(const string & out);

/**
 * @brief Write an integer to the debug output file.
 *
 * @param out The integer to write to the debug output file.
 */
void writeToDebug(const int & out);

/**
 * @brief Write a character to the debug output file.
 *
 * @param out The character to write to the debug output file.
 */
void writeToDebug(const char & out);

/**
 * @brief Calculate the end time given a start time and duration in minutes.
 *
 * @param start The start time.
 * @param duration The duration in minutes.
 * @return The end time.
 */
Datetime calculateEndtime(const Datetime & start, long long duration);

/**
 * @brief Convert seconds to a Datetime object.
 *
 * @param seconds The number of seconds.
 * @return The corresponding Datetime object.
 */
Datetime secondsToDatetime(long long seconds);

/**
 * @brief Convert a formatted string to an Event repeat type.
 *
 * @param formatted The formatted string representing the Event repeat.
 * @return The corresponding Event repeat type.
 */
Event::Repeat stringToRepeat(const string & formatted);

/**
 * @brief Convert an Event repeat type to a string.
 *
 * @param repeat The Event repeat type.
 * @return The string representation of the Event repeat.
 */
/**
 * @brief Convert an Event repeat type to a string representation.
 *
 * @param repeat The Event repeat type.
 * @return The string representation of the Event repeat.
 */
string repeatToString(const Event::Repeat & repeat);

/**
 * @brief Convert an Event repeat type to the corresponding number of seconds.
 *
 * @param repeat The Event repeat type.
 * @return The number of seconds represented by the Event repeat.
 */
long long repeatToSeconds(const Event::Repeat & repeat);

/**
 * @brief Find the date and time of yesterday given a specific date and time.
 *
 * @param dt The reference date and time.
 * @return The date and time of yesterday.
 */
Datetime findYesterday(const Datetime & dt);

/**
 * @brief Find the date and time of tomorrow given a specific date and time.
 *
 * @param dt The reference date and time.
 * @return The date and time of tomorrow.
 */
Datetime findTomorrow(const Datetime & dt);

/**
 * @brief Find the start of the next week given a specific date and time.
 *
 * @param dt The reference date and time.
 * @return The start of the next week.
 */
Datetime startOfNextWeek(const Datetime & dt);

/**
 * @brief Find the end of the current week given a specific date and time.
 *
 * @param dt The reference date and time.
 * @return The end of the current week.
 */
Datetime endOfCurrentWeek(const Datetime & dt);

/**
 * @brief Find the start of the previous week given a specific date and time.
 *
 * @param dt The reference date and time.
 * @return The start of the previous week.
 */
Datetime startOfPreviosWeek(const Datetime & dt);

/**
 * @brief Find the start of the current week given a specific date and time.
 *
 * @param dt The reference date and time.
 * @return The start of the current week.
 */
Datetime startOfCurrentWeek(const Datetime & dt);

/**
 * @brief Get user input from a window.
 *
 * @param win The window to read input from.
 * @return The user input as a string.
 */
string getInput(WINDOW * win);

/**
 * @brief Prompt the user for input with a specific question in a window.
 *
 * @param win The window to prompt for input.
 * @param question The question to display.
 * @return The user input as a string.
 */
string promptInput(WINDOW * win, string question);

#endif // UTIL_H
