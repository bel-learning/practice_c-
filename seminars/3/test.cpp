#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;
    int timeToSeconds() {
        return (m_Hour * 3600 + m_Minute * 60 + m_Second );
    }
    CTime generateTimeFromSeconds(int sec) {
        CTime new_time(sec / 3600, (sec % 3600) / 60, (sec % 3600) % 60);
        return new_time;
    }
public:
    // constructor, destructor
    CTime(int sec) {
        CTime tmp = this->generateTimeFromSeconds(sec);
        m_Hour = tmp.m_Hour;
        m_Minute = tmp.m_Minute;
        m_Second = tmp.m_Second;
    }
    CTime(int hour, int minute) {
        if(hour > 23 || hour < 0) throw invalid_argument("Invalid time format.");
        if(minute > 59 || minute < 0) throw invalid_argument("Invalid time format.");
        m_Hour = hour;
        m_Minute = minute;
        m_Second = 0;

    }
    CTime(int hour, int minute, int second) {
        if(hour > 23 || hour < 0) throw invalid_argument("Invalid time format.");
        if(minute > 59 || minute < 0) throw invalid_argument("Invalid time format.");
        if(second > 59 || second < 0) throw invalid_argument("Invalid time format.");
        m_Hour = hour;
        m_Minute = minute;
        m_Second = second;

    }

    // arithmetic operators

    // comparison operators

    // output operator
    friend bool operator + (CTime a, CTime b);
    friend bool operator - (CTime a, CTime b);
    friend bool operator == (CTime a, CTime b);
    friend bool operator != (CTime a, CTime b);
    friend bool operator > (CTime a, CTime b);
    friend bool operator >= (CTime a, CTime b);
    friend bool operator < (CTime a, CTime b);
    friend bool operator <= (CTime a, CTime b);
    friend ostream & operator << (ostream &, CTime);
    

    CTime operator ++ () {};
    CTime operator ++ (int) {};

    CTime operator -- () {};
    CTime operator -- (int) {};


    friend class ::CTimeTester;
};

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour 
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );

    return 0;
}
#endif /* __PROGTEST__ */
