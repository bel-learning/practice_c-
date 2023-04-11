#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

typedef long long ll;
// uncomment if your code implements initializer lists
#define EXTENDED_SYNTAX

class CRange
{
public:
  // constructor
  CRange() {};
  CRange(ll low, ll high)
  {
    if (low > high)
      throw logic_error("");
    _low = low;
    _high = high;
  }

  bool operator<(const CRange & b) const
  {
    return _low < b._low;
  }
 
  
  ll &getHigh()
  {
    return _high;
  }
  ll &getLow()
  {
    return _low;
  }
  ll constGetLow() const { return _low;};

  ll constGetHigh() const { return _high;};


  friend ostringstream & operator << (ostringstream & out, const CRange & range) {
    ios_base::fmtflags o_flags = out.flags();

    out << dec;
    if(range.constGetHigh() == range.constGetLow()) 
      out << range.constGetHigh();
    else 
      out << '<' << range.constGetLow() << ".." << range.constGetHigh() << '>';
    out.flags(o_flags);
    return out;
  }



  friend ostream & operator << (ostream & out, const CRange & range) {
    ios_base::fmtflags o_flags = out.flags();
    
    out << dec;
    if(range.constGetHigh() == range.constGetLow()) 
      out << range.constGetHigh();
    else 
      out << '<' << range.constGetLow() << ".." << range.constGetHigh() << '>';
    out.flags(o_flags);
    return out;
  }
private:
  // todo
  ll _low;
  ll _high;
};

class CRangeList
{
public:
  // constructor
  CRangeList(){};

  CRangeList(const initializer_list<CRange> & list){
    for(const CRange & r : list) {
      (*this) += r;
    }
  };
  class const_iterator {
    public:
        const_iterator(const CRange* ptr) : m_ptr(ptr) {}
        
        const CRange& operator*() const {
            return *m_ptr;
        }
        
        const CRange* operator->() const {
            return m_ptr;
        }
        
        const_iterator& operator++() {
            ++m_ptr;
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        friend bool operator==(const const_iterator& a, const const_iterator& b) {
            return a.m_ptr == b.m_ptr;
        }
        
        friend bool operator!=(const const_iterator& a, const const_iterator& b) {
            return !(a.m_ptr == b.m_ptr);
        }
    
      private:
          const CRange* m_ptr;
      };
      
  const_iterator begin() const {
      return const_iterator(_intervals.data());
  }
  
  const_iterator end() const {
      return const_iterator(_intervals.data() + _intervals.size());
  }
  // includes long long / range
  bool includes(const long long & num) const
  {
    CRange interval(num, num);
    return includes(interval);
  }
  bool includes(const CRange & interval) const
  {
    if(_intervals.empty()) return false;
    vector<CRange>::const_iterator it = lower_bound(_intervals.begin(), _intervals.end(), interval);
    if(it == _intervals.end())  {
      // cout << "at end" << endl;
      const CRange & lastInterval = _intervals.back();
      // cout <<  "last interval high: " << lastInterval.constGetHigh() << endl;
      // cout <<  "interval low: " << interval.getLow() << endl;
      return interval.constGetLow() >= lastInterval.constGetLow() && interval.constGetHigh() <= lastInterval.constGetHigh();
    }
    else if (it == _intervals.begin()) {
      return interval.constGetHigh() <= it->constGetHigh() && interval.constGetLow() >= it->constGetLow();
    }
    else {
      const auto& leftNeighbor = *(it - 1);
      const auto& rightNeighbor = *it;
      // cout << it->constGetLow() << " - " << it->constGetHigh() << endl;
      if (interval.constGetLow() >= leftNeighbor.constGetLow() && interval.constGetHigh() <= leftNeighbor.constGetHigh()) {
          return true;
      }
      if (interval.constGetLow() >= rightNeighbor.constGetLow() && interval.constGetHigh() <= rightNeighbor.constGetHigh()) {
          return true;
      }
      return false;
    }
  }
  // += range / range list
  CRangeList & operator += (const CRangeList & other)
  {
    for(size_t i = 0; i < other._intervals.size(); i++) {
      *this = (*this + other._intervals[i]);
    }
    return *this;
  }

  CRangeList & operator += (const CRange & interval)
  {
    *this = (*this + interval);
    return *this;
  }

  // -= range / range list
   CRangeList & operator -= (const CRangeList & other)
  {
    for(size_t i = 0; i < other._intervals.size(); i++) {
      *this = (*this - other._intervals[i]);
    }
    return *this;
  }
   CRangeList & operator -= (const CRange & interval)
  {
    *this = (*this - interval);
    return *this;
  }
  // = range / range list
  CRangeList & operator = (const CRangeList & range_list)
  {
    _intervals = range_list._intervals;
    return *this;
  }
  CRangeList & operator = (const CRange & range)
  {
    _intervals.clear();
    _intervals.push_back(range);
    return *this;
  }
  // operator ==
  friend bool operator == (const CRangeList & a, const CRangeList & b) {

    if(a._intervals.size() != b._intervals.size()) return false;
    for(size_t i = 0; i < a._intervals.size(); i++) {
      if(a._intervals[i].constGetLow() != b._intervals[i].constGetLow()) return false;
      if(a._intervals[i].constGetHigh() != b._intervals[i].constGetHigh()) return false;
    }
    return true;
  }
  // operator !=
  friend bool operator != (const CRangeList & a, const CRangeList & b) {
    return !(a==b);
  }
  // operator <<

  CRangeList operator + (const CRange & interval)
  {
    CRangeList new_interval = *this;
    if (new_interval._intervals.empty())
    {
      new_interval._intervals.push_back(interval);
      return new_interval;
    }

    vector<CRange>::const_iterator pos = new_interval.findPosition(interval);
    new_interval._intervals.insert(pos, interval);
    new_interval.mergeIntervals();
    return new_interval;
  }
  
  CRangeList operator + (const CRangeList & other)
  {
    CRangeList new_intervals = *this;
    for(size_t i = 0; i < other._intervals.size(); i++) {
      new_intervals += other._intervals[i];
    }
    return new_intervals;
  }

  CRangeList operator - (const CRange & remove)
  {
    if (_intervals.empty())
      return *this;

    // Look left and right
    vector<CRange> res;
    for(size_t i = 0; i < _intervals.size(); i++) {
      // no overlap (-300 < -400) || (-500 > -400)
      if(_intervals[i].constGetHigh() < remove.constGetLow() || _intervals[i].constGetLow() > remove.constGetHigh()) {
        res.push_back(_intervals[i]);
      }
      else if(_intervals[i].constGetLow() < remove.constGetLow() && _intervals[i].constGetHigh() > remove.constGetHigh()) {
        if(remove.constGetLow() != LLONG_MIN)
          res.push_back({_intervals[i].constGetLow(), remove.constGetLow() - 1});
        if(remove.constGetHigh() != LLONG_MAX)
          res.push_back({remove.constGetHigh() + 1, _intervals[i].constGetHigh()});
      }
      else if (_intervals[i].constGetLow() < remove.constGetLow()) {
          // overlap on the left side, update end
          if(remove.constGetLow() != LLONG_MIN)
            res.push_back(CRange(_intervals[i].constGetLow(), remove.constGetLow() - 1));
      }
      else if (_intervals[i].constGetHigh() > remove.constGetHigh()) {
          // overlap on the right side, update start
          if(remove.constGetHigh() != LLONG_MAX)
            res.push_back(CRange(remove.constGetHigh() + 1, _intervals[i].constGetHigh()));
        }
    }
    _intervals = res;
    return *this;
  }
  CRangeList operator - (const CRangeList & other)
  {
    for(size_t i = 0; i < other._intervals.size(); i++) {
      *this -= other._intervals[i];
    }
    return *this;
  }

  vector<CRange>::const_iterator findPosition(const CRange &interval) const
  {
    vector<CRange>::const_iterator lower = lower_bound(_intervals.begin(), _intervals.end(), interval);
    return lower;
  }
  void mergeIntervals()
  {
    vector<CRange> merged;
    for (size_t i = 0; i < _intervals.size(); i++)
    {
      if(!merged.empty() && merged.back().getHigh() == LLONG_MAX) break;
      if (merged.empty() || merged.back().getHigh()+1 < _intervals[i].getLow())
      {
        merged.push_back(_intervals[i]);
      }
      else
      {
        merged.back().getHigh() = max(_intervals[i].getHigh(), merged.back().getHigh());
      }
    }
    _intervals = merged;
  }

  // void printIntervals()
  // {
  //   for (size_t i = 0; i < _intervals.size(); i++)
  //   {
  //     cout << "(" << _intervals[i].getLow() << " - " << _intervals[i].getHigh() << ") ";
  //   }
  //   cout << endl;
  // }

  vector<CRange> getIntervals() const {
    return _intervals;
  }
  friend ostringstream & operator << (ostringstream &out, const CRangeList &list)
  {
    ios_base::fmtflags o_flags = out.flags();
    vector<CRange> intervals = list.getIntervals();
    out << dec;
    out << "{";
    for(size_t i = 0; i < intervals.size(); i++) {
      ll low_num = intervals[i].getLow();
      ll high_num = intervals[i].getHigh();
      if(low_num == high_num)
        out << low_num;
      else
        out << "<" << low_num << ".." << high_num << ">";
      if(i != intervals.size() - 1) 
        out << ',';
    }
    out << "}";
    out.flags(o_flags);
    return out;
  }
  friend ostream & operator << (ostream &out, const CRangeList &list)
  {
    ios_base::fmtflags o_flags = out.flags();
    vector<CRange> intervals = list.getIntervals();
    out << dec;
    out << "{";
    for(size_t i = 0; i < intervals.size(); i++) {
      ll low_num = intervals[i].getLow();
      ll high_num = intervals[i].getHigh();
      if(low_num == high_num)
        out << low_num;
      else
        out << "<" << low_num << ".." << high_num << ">";
      if(i != intervals.size() - 1) 
        out << ',';
    }
    out << "}";
    out.flags(o_flags);
    return out;
  }


private:
  // todo
  vector<CRange> _intervals;
};


CRangeList operator + (const CRange & range1, const CRange & range2) 
{
  CRangeList intervals;
  intervals += range1;
  intervals += range2;
  return intervals;
}
CRangeList operator - (const CRange & range1, const CRange range2)
  {
    CRangeList intervals;
    intervals += range1;
    intervals -= range2;
    return intervals;
  }




#ifndef __PROGTEST__
string toString(const CRangeList &x)
{
  ostringstream oss;
  oss << x;
  return oss.str();
}

int main(void)
{
  CRangeList a, b;

  assert(sizeof(CRange) <= 2 * sizeof(long long));
  a = CRange(5, 10);
  a += CRange(25, 100);
  assert(toString(a) == "{<5..10>,<25..100>}");
  a += CRange(-5, 0);
  a += CRange(8, 50);
  assert(toString(a) == "{<-5..0>,<5..100>}");
  a += CRange(101, 105) + CRange(120, 150) + CRange(160, 180) + CRange(190, 210);
  assert(toString(a) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}");
  a += CRange(106, 119) + CRange(152, 158);
  assert(toString(a) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}");
  a += CRange(-3, 170);
  a += CRange(-30, 1000);
  assert(toString(a) == "{<-30..1000>}");
  b = CRange(-500, -300) + CRange(2000, 3000) + CRange(700, 1001);
  a += b;
  assert(toString(a) == "{<-500..-300>,<-30..1001>,<2000..3000>}");
  a -= CRange(-400, -400);
  assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}");
  a -= CRange(10, 20) + CRange(900, 2500) + CRange(30, 40) + CRange(10000, 20000);
  assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
  try
  {
    a += CRange(15, 18) + CRange(10, 0) + CRange(35, 38);
    assert("Exception not thrown" == nullptr);
  }
  catch (const logic_error &e)
  {
  }
  catch (...)
  {
    assert("Invalid exception thrown" == nullptr);
  }
  assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
  b = a;
 
  assert(a == b);
  assert(!(a != b));
  b += CRange(2600, 2700);
  assert(toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
  assert(a == b);
  assert(!(a != b));
  b += CRange(15, 15);
  assert(toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}");
  assert(!(a == b));
  assert(a != b);
  assert(b.includes(15));
  assert(b.includes(2900));
  assert(b.includes(CRange(15, 15)));
  assert(b.includes(CRange(-350, -350)));
  assert(b.includes(CRange(100, 200)));
  assert(!b.includes(CRange(800, 900)));
  // cout << toString(b) << endl;
  assert(!b.includes(CRange(-1000, -450)));
  assert(!b.includes(CRange(0, 500)));
  a += CRange(-10000, 10000) + CRange(10000000, 1000000000);
  assert(toString(a) == "{<-10000..10000>,<10000000..1000000000>}");
  b += a;
  assert(toString(b) == "{<-10000..10000>,<10000000..1000000000>}");
  b -= a;
  assert(toString(b) == "{}");
  b += CRange(0, 100) + CRange(200, 300) - CRange(150, 250) + CRange(160, 180) - CRange(170, 170);
  assert(toString(b) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
  b -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
  assert(toString(b) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");

  CRangeList c;
  c += CRange(LLONG_MAX-4, LLONG_MAX-1);
  assert(!c.includes(CRange(LLONG_MAX-1, LLONG_MAX)));

// #ifdef EXTENDED_SYNTAX
  CRange p{2,3};
  CRangeList x{{5, 20}, {150, 200}, {-9, 12}, {48, 93}};
  

  assert(toString(x) == "{<-9..20>,<48..93>,<150..200>}");
  ostringstream oss;
  oss << setfill('=') << hex << left;
  

  for (const auto &v : x + CRange(-100, -100)) {
    oss << v << endl;
  }
  oss << setw(10) << 1024;
  // cout << oss.str() << endl;
  assert(oss.str() == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======");
  cout << endl;



// #endif /* EXTENDED_SYNTAX */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
