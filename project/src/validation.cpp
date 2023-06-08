#include <string>
#include <cctype>
#include "datetime.h"
#include "util.h"

using namespace std;

/**
 * @file validation.cpp
 *
 * @brief Declaration of validation functions to validate various inputs.
 */

bool isValidType(const string &input)
{
    if (input != "deadline" && input != "task" && input != "event" && input != "exit")
        return false;
    return true;
}

bool isValidTitle(const string &input)
{
    return input.size() > 0 && input.size() < 80;
}
bool isValidDescription(const string &input)
{
    return input.size() < 80;
}

bool isValidRepeat(const string &input)
{
    if (input != "yearly" && input != "monthly" && input != "weekly" && input != "biweekly" && input != "daily" && input != "exit" && input != "" && input != "none")
        return false;
    return true;
}
bool isValidDate(const string &input)
{

    int occur = 0;
    // Char should either be - or digit
    for (size_t i = 0; i < input.size(); i++)
    {
        if (input[i] == '-')
        {
            occur++;
        }
        else
        {
            if (!isdigit(input[i]))
            {
                return false;
            }
        }
    }
    // Too many - or lack of -
    if (occur != 4 && occur != 3 && occur != 2 && occur != 1)
        return false;

    Datetime assumedDate(getCurrentDateTime());
    istringstream iss(input);
    string token;
    // If we have 4 - symbol, then format should be YYYY-MM-DD-hh-mm, validate the year
    if (occur == 4)
    {
        // Validate if it's valid year

        getline(iss, token, '-');
        if (token.empty())
            return false;
        if (token.size() > 4)
            return false;
        for (auto c : token)
            if (!isdigit(c))
                return false;
        assumedDate.year = stoi(token);
        occur--;
    }
    // If we have 3 - symbol, then format should be MM-DD-hh-mm, validate the month
    if (occur == 3)
    {
        getline(iss, token, '-');
        if (token.empty())
            return false;
        if (token.size() > 2)
            return false;
        for (auto c : token)
            if (!isdigit(c))
                return false;
        if (stoi(token) > 12 || stoi(token) < 0)
            return false;
        assumedDate.month = stoi(token);
        occur--;
    }
    // If we have 2 - symbol, then format should be DD-hh-mm, validate the day respect with month and year
    if (occur == 2)
    {
        getline(iss, token, '-');
        if (token.empty())
            return false;
        if (token.size() > 2)
            return false;
        for (auto c : token)
            if (!isdigit(c))
                return false;
        int daysInMonth = getDaysInMonth(assumedDate);

        if (stoi(token) > daysInMonth || stoi(token) < 0)
            return false;
        assumedDate.day = stoi(token);
        occur--;
    }
    // If we have 1 - symbol, then format should be hh-mm, validate the hours
    if (occur == 1)
    {
        getline(iss, token, '-');
        if (token.empty())
            return false;
        if (token.size() > 2)
            return false;
        for (auto c : token)
            if (!isdigit(c))
                return false;
        if (stoi(token) > 23 || stoi(token) < 0)
            return false;
        assumedDate.hour = stoi(token);
        occur--;
    }

    // It's propagation of others, there's no mm format in addEvent. validate the minute
    if (occur == 0)
    {
        getline(iss, token, '-');
        if (token.empty())
            return false;
        if (token.size() > 2)
            return false;
        for (auto c : token)
            if (!isdigit(c))
                return false;
        if (stoi(token) > 59 || stoi(token) < 0)
            return false;
        assumedDate.minute = stoi(token);
        occur--;
    }

    return true;
}

bool isValidMinutes(const std::string &input)
{
    if (input.size() == 0)
        return false;
    for (const auto c : input)
    {
        if (!isdigit(c))
            return false;
    }
    if (stoi(input) == 0)
        return false;
    if (input.size() > 5)
        return false;
    if (stoi(input) > 1440)
    {
        return false;
    }
    return true;
}
bool isValidBool(const std::string &input)
{
    if (input.size() != 1)
        return false;
    if (input[0] != '1' && input[0] != '0')
        return false;
    return true;
}

bool isValidLocation(const std::string &input)
{
    return input.size() < 40;
}