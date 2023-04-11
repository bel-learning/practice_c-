#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */




class CTimeStamp
{
  // TODO
  int year, month, day, hour, min, second;

public:
  CTimeStamp(int y = -1, int mm = 0 , int d = 0, int h = 0, int m = 0, int s = 0) {
    year = y; month = mm; day = d; hour = h; min = m; second = s;
  }
  int getYear() const { return year; }
  int getMonth() const { return month; }
  int getDay() const { return day; }
  int getHour() const { return hour; }
  int getMin() const { return min; }
  int getSec() const { return second; }

  bool isWithinTimestamp(CTimeStamp start, CTimeStamp end) const  {
    if (year < start.getYear() || year > end.getYear()) {
          return false;
      }
    else if (year == start.getYear() && month < start.getMonth()) {
        return false;
    }
    else if (year == end.getYear() && month > end.getMonth()) {
        return false;
    }
    else if (year == start.getYear() && month == start.getMonth() && day < start.getDay()) {
        return false;
    }
    else if (year == end.getYear() && month == end.getMonth() && day > end.getDay()) {
        return false;
    }
    else if (year == start.getYear() && month == start.getMonth() && day == start.getDay() && hour < start.getHour()) {
        return false;
    }
    else if (year == end.getYear() && month == end.getMonth() && day == end.getDay() && hour > end.getHour()) {
        return false;
    }
    else if (year == start.getYear() && month == start.getMonth() && day == start.getDay() && hour == start.getHour() && min < start.getMin()) {
        return false;
    }
    else if (year == end.getYear() && month == end.getMonth() && day == end.getDay() && hour == end.getHour() && min > end.getMin()) {
        return false;
    }
    else if (year == start.getYear() && month == start.getMonth() && day == start.getDay() && hour == start.getHour() && min == start.getMin() && second < start.getSec()) {
        return false;
    }
    else if (year == end.getYear() && month == end.getMonth() && day == end.getDay() && hour == end.getHour() && min == end.getMin() && second > end.getSec()) {
        return false;
    }
    return true;
};

};
  

class CContact
{
  // TODO
  CTimeStamp _stamp; int _phone1, _phone2;
public:
  CContact(CTimeStamp stamp, int phone1, int phone2) : _stamp(stamp), _phone1(phone1), _phone2(phone2){};
  int getPhone1() const {
    return _phone1;
  }
  int getPhone2() const {
    return _phone2;
  }
  CTimeStamp getStamp() const {
    return _stamp;
  }
};
class CEFaceMask
{
  // TODO
  vector<CContact> contacts;
public:
  CEFaceMask & addContact(CContact contact) {
    contacts.push_back(contact);
    
    return *this;
  }
  
  vector<int> listContacts(int number, CTimeStamp start = CTimeStamp(-1,0,0,0,0,0), CTimeStamp end = CTimeStamp(-1,0,0,0,0,0)) const {
    if(start.getYear() == -1) {
      // only number is provided
      
      vector<int> contacted;
      for(size_t i = 0; i < contacts.size(); i++) {
        int phone1 = contacts[i].getPhone1();
        int phone2 = contacts[i].getPhone2();

        if(phone1 == number) {
          bool exists = false;
          for(size_t i = 0; i < contacted.size(); i++) {
            if(contacted[i] == phone2) {
                exists = true;
                break;
            }
          }
          if(!exists && phone2 != number)
            contacted.push_back(phone2);
        }
        
        if(phone2 == number) {
          bool exists = false;
          for(size_t i = 0; i < contacted.size(); i++) {
            if(contacted[i] == phone1) {
                exists = true;
                break;
            }
          }
          if(!exists && phone1 != number)
            contacted.push_back(phone1);
        }
     
      }
      // for(size_t i = 0; i < contacted.size(); i++) {
      //   cout << contacted[i] << " ";
      // }
      // cout << endl;
      return contacted;
    }
    else {
      // only there's a stamp
      vector<int> contacted;

      for(size_t i = 0; i < contacts.size(); i++) {
        int phone1 = contacts[i].getPhone1();
        int phone2 = contacts[i].getPhone2();

        if(!contacts[i].getStamp().isWithinTimestamp(start, end)) {
            continue;
          }
        
        if(phone1 == number) {
          bool exists = false;
          
          for(size_t i = 0; i < contacted.size(); i++) {
            if(contacted[i] == phone2) {
                exists = true;
            }
          }
          if(!exists && phone2 != number)
            contacted.push_back(phone2);
        }
        
        if(phone2 == number) {
          bool exists = false;
          for(size_t i = 0; i < contacted.size(); i++) {
            if(contacted[i] == phone1) {
                exists = true;
            }
          }
          if(!exists && phone1 != number)
            contacted.push_back(phone1);
        }
      }

      return contacted;
    }
  }
};


#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;
  
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
 
  CEFaceMask test1;
  // test.listContacts();
  return 0;
}
#endif /* __PROGTEST__ */
