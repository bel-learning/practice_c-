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
  private:
  int m_year;
  int m_month;
  int m_day;
  int m_hour;
  int m_minute;
  int m_second;
  public:
  CTimeStamp(int year,int month, int day , int hour , int minute , int second):
  m_year(year),m_month(month),m_day(day),m_hour(hour),m_minute(minute),m_second(second){
  }
  int ChangetoUnix()const{
    return (m_year-2000) * 31536000 + (m_month * 2592000) + (m_day * 86400 ) + (m_hour * 3600) + (m_minute * 60) + m_second;
  }
  // bool isbetween(CTimeStamp start , CTimeStamp end) const {
  //   if(m_year > start.m_year && m_year < end.m_year){
  //     return true;
  //   }
  //   else if( m_year == start.m_year && m_year == end.m_year){
  //     if(m_month > start.m_month && m_month < end.m_month){
  //       return true;
  //     }
  //     else if(m_month == start.m_month && m_month == end.m_month){
  //       if(m_day > start.m_day && m_day < end.m_day){
  //         return true;
  //       }
  //       else if(m_day == start.m_day && m_day == end.m_day){
  //         if(m_hour > start.m_hour && m_hour < end.m_hour){
  //           return true;
  //         }
  //         else if(m_hour == start.m_hour && m_hour == end.m_hour){
  //           if(m_minute > start.m_minute && m_minute < end.m_minute){
  //             return true;
  //           }
  //           else if(m_minute == start.m_minute && m_minute == end.m_minute){
  //             if(m_second >= start.m_second && m_second <= end.m_second){
  //               return true;
  //             }
  //           }
  //         }
  //       }
  //     }
  //   }
  

  //   return false;

  // }
  bool isbetween(CTimeStamp start, CTimeStamp end) const{
    int middle = this->ChangetoUnix();
    int from = start.ChangetoUnix();
    int to = end.ChangetoUnix();
    return middle >= from &&  middle <= to;
  }
};


class CContact
{
  // TODO
  private:
  CTimeStamp m_Stamp;
  int m_ph1;
  int m_ph2;
  public:
  CContact(CTimeStamp stamp , int ph1 , int ph2):
  m_Stamp(stamp),m_ph1(ph1),m_ph2(ph2){
  }
  int isSame( int phonenumber)const {
    if(phonenumber == m_ph1 && phonenumber == m_ph2){
      return 0;
    }
    if(phonenumber==m_ph1){
      return m_ph2;
    }
    else if(phonenumber==m_ph2){
      return m_ph1;
    }
    return 0;
  }

  int isSametime(int phonenumber,CTimeStamp start,CTimeStamp end)const{
    if(phonenumber == m_ph1 && phonenumber == m_ph2){
      return 0;
    }
    if(phonenumber == m_ph1 && m_Stamp.isbetween(start,end)){
      return m_ph2;
    }
    else if(phonenumber==m_ph2 && m_Stamp.isbetween(start,end)){
      return m_ph1;
    }
    return 0;
  }

  friend ostream & operator << (ostream & os , const CContact & contact){
    return os << "phone1: " << contact.m_ph1 << ", phone2: " << contact.m_ph2 ;
  }

};

class CEFaceMask
{
  // TODO
  private:
    vector<CContact> db;
  public:
  CEFaceMask & addContact(CContact newcontact)
  {
    db.push_back(newcontact);
    // for(const CContact contact : db){
    //   cout << contact << endl;
    // }
    return *this;
  };

  static void printvector ( vector<int> array){
    for(unsigned int i = 0 ; i < array.size() ; ++i){
      cout << array[i] << " ";
    }
    cout << endl;
  };

  vector<int> listContacts(int phonenumber) const
  {
    vector<int> list;
    for(unsigned int i =0 ; i < db.size(); i++){
      int result = db[i].isSame(phonenumber);
      if(result){
        bool contained = false;
        for(unsigned int j = 0 ; j < list.size();j++){
          if(result == list[j]){
            contained = true;
          }
        }
        if(!contained){
          list.push_back(result);
        }
      }
    }
    // printvector(list);
    return list;
  };
  vector<int>listContacts(int phonenumber,CTimeStamp start, CTimeStamp end) const
  {
    vector<int> list;
    for(unsigned int i = 0 ; i < db.size() ; i++){
      int result = db[i].isSametime(phonenumber,start,end);
      if(result){
        bool contained = false;
        for(unsigned int j = 0 ; j  < list.size() ; j++){
          if(result == list[j]){
            contained = true;
          }
        }
        if(!contained){
          list.push_back(result);
        }
      }
    }
    return list;
  };
};

void printvector ( vector<int> array){
    for(unsigned int i = 0 ; i < array.size() ; ++i){
      cout << array[i] << " ";
    }
    cout << endl;
}

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


  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}));
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  return 0;
}
#endif /* __PROGTEST__ */
