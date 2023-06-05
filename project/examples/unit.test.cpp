#include <cassert>
#include "../src/util.h"
#include "../src/datetime.h"

int main()
{
    {
        const Datetime time(2020, 2, 29, 0, 0, 0);
        assert(findTomorrow(time) == Datetime(2020, 3, 1, 0, 0, 0));
    }
    // Test dayIndexInWeek
    // Test Case 1
    {
        Datetime time(2023, 6, 5, 0, 0, 0);
        assert(dayIndexInWeek(time) == 1);
    }

    // Test Case 2
    {
        Datetime time(2023, 6, 6, 0, 0, 0);
        assert(dayIndexInWeek(time) == 2);
    }

    // Test Case 3
    {
        Datetime time(2023, 6, 7, 0, 0, 0);
        assert(dayIndexInWeek(time) == 3);
    }

    // Test Case 4
    {
        Datetime time(2023, 6, 8, 0, 0, 0);
        assert(dayIndexInWeek(time) == 4);
    }

    // Test Case 5
    {
        Datetime time(2023, 6, 9, 0, 0, 0);
        assert(dayIndexInWeek(time) == 5);
    }

    // Test Case 6
    {
        Datetime time(2023, 6, 10, 0, 0, 0);
        assert(dayIndexInWeek(time) == 6);
    }

    // Test Case 7
    {
        Datetime time(2023, 6, 11, 0, 0, 0);
        assert(dayIndexInWeek(time) == 0);
    }

    // Test startOfCurrentWeek
    // Monday
    {
        Datetime dt(2023, 6, 5, 12, 0, 0);
        Datetime expected(2023, 6, 5, 0, 0, 0);
        assert(startOfCurrentWeek(dt) == expected);
    }
    // Sunday
    {
        Datetime dt(2023, 6, 11, 18, 30, 45);
        Datetime expected(2023, 6, 5, 0, 0, 0);
        assert(startOfCurrentWeek(dt) == expected);
    }
    // Wednesday
    {
        Datetime dt(2023, 6, 7, 8, 15, 30);
        Datetime expected(2023, 6, 5, 0, 0, 0);
        assert(startOfCurrentWeek(dt) == expected);
    }
    // Saturday
    {
        Datetime dt(2023, 6, 10, 23, 59, 59);
        Datetime expected(2023, 6, 5, 0, 0, 0);
        assert(startOfCurrentWeek(dt) == expected);
    }
    // Tuesday
    {
        Datetime dt(2023, 6, 6, 6, 30, 0);
        Datetime expected(2023, 6, 5, 0, 0, 0);
        assert(startOfCurrentWeek(dt) == expected);
    }

    // Test calculateEndTime

    {
        Datetime start(2023, 6, 5, 12, 0, 0);
        long long duration = 120; // 2 hours
        Datetime expected(2023, 6, 5, 14, 0, 0);
        assert(calculateEndtime(start, duration) == expected);
    }

    {
        Datetime start(2023, 6, 11, 18, 30, 0);
        long long duration = 90; // 1 hour and 30 minutes
        Datetime expected(2023, 6, 11, 20, 0, 0);
        assert(calculateEndtime(start, duration) == expected);
    }

    {
        Datetime start(2023, 6, 7, 8, 15, 0);
        long long duration = 60; // 1 hour
        Datetime expected(2023, 6, 7, 9, 15, 0);
        assert(calculateEndtime(start, duration) == expected);
    }

    {
        Datetime start(2023, 6, 10, 23, 59, 0);
        long long duration = 120; // 2 hours
        Datetime expected(2023, 6, 11, 1, 59, 0);
        assert(calculateEndtime(start, duration) == expected);
    }

    {
        Datetime start(2023, 6, 6, 6, 30, 0);
        long long duration = 45; // 45 minutes
        Datetime expected(2023, 6, 6, 7, 15, 0);
        assert(calculateEndtime(start, duration) == expected);
    }
    return 0;
}