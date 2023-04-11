#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CEmployee
{
public:
  CEmployee(const string &name, const string &surname, const string &email, unsigned int salary) 
  : m_name(name), m_surname(surname), m_email(email), m_salary(salary)
  {
  }

  bool isSame(const CEmployee &employee) const
  {
    return isSame(employee.m_name,employee.m_surname) || isSame(employee.m_email);
  }

  bool isSame(const string &name, const string &surname) const
  {
    if (m_name == name && m_surname == surname)
    {
      return true;
    }
    return false;
  }

  bool isSame(const string & email) const
  {
    return m_email == email;
  }

  void changebyname(const string & name, const string & surname)
  {
    m_name = name;
    m_surname = surname;
  }

  void changebyemail (const string & email)
  {
    m_email = email;
  }

  void changebysalary(unsigned int salary) 
  {
    m_salary = salary;
  }

  unsigned int giveSalary() const
  {
    return m_salary;
  }

  bool comparebySalary(const CEmployee & other) const
  {
    return m_salary < other.m_salary;
  }

  bool comparebyName (const CEmployee & other) const
  {
    if(m_name != other.m_name) return m_name < other.m_name;
    return m_surname < other.m_surname;
  }

  bool comparebyEmail (const CEmployee & other) const
  {
    return m_email < other.m_email;
  }
 
  friend ostream & operator << (ostream & os , const CEmployee & employee){
    return os << "Name: " << employee.m_name << " ,Surname: " << employee.m_surname << " ,Email: " << employee.m_email << " ,Salary: " << employee.m_salary << endl;
  }

private:
  string m_name;
  string m_surname;
  string m_email;
  unsigned int m_salary;
};

class CPersonalAgenda
{
public:
  // CPersonalAgenda  ( void );
  // ~CPersonalAgenda ( void );

  //adding employee to the database
  bool add(const string &name,const string &surname,const string &email,unsigned int salary)
  {
    CEmployee employee(name, surname, email, salary);
    for (size_t i = 0; i < db.size(); ++i)
    {
      if (db[i].isSame(employee))
      {
        return false;
      }
    }
    db.push_back(employee);
    return true;
  };



  bool del(const string &name,
           const string &surname)
  {
    for(size_t i = 0 ; i < db.size() ; ++i)
    {
      if(db[i].isSame(name,surname)){
        db[i] = db.back();
        db.pop_back();
        return true;
      }
    }
    return false;
  }

  bool del(const string &email)
  {
    for(size_t i = 0 ; i < db.size() ; ++ i)
    {
      if(db[i].isSame(email)){
        db[i] = db.back();
        db.pop_back();
        return true;
      }
    }
    return false;
  }

  // employee cannot be found or newname and newSurname is not unique
  bool changeName(const string &email,
                  const string &newName,
                  const string &newSurname)
  { 
    bool unique = true;
    for(size_t i = 0 ; i < db.size();++i){
      if(db[i].isSame(newName,newSurname)){
        unique = false;
      }
    }
    if(unique){
      for(size_t i = 0 ; i < db.size();++i)
      {
        if(db[i].isSame(email))
        { 
          db[i].changebyname(newName,newSurname);
          return true;  
        }
      }
    }
    return false;
  }


  bool changeEmail(const string &name,
                   const string &surname,
                   const string &newEmail)
  {
    for(size_t i = 0 ; i < db.size() ; ++i){
      if(db[i].isSame(name,surname)){
        db[i].changebyemail(newEmail);
        return true;
      }
    }
    return false;
  }

// update the salary of the given employee. 
// There are two variants of the method -
//  the employee is identified either by the name and surname, or by the e-mail.
//  The methods return true for success or false for a failure (employee not found).
  bool setSalary(const string &name,
                 const string &surname,
                 unsigned int salary)
  { 
    sortbyname();
    vector<CEmployee>::iterator it = lower_bound(db.begin(),db.end(),make_pair(name,surname),[](const CEmployee & a , const pair<string,string> & p){
      return !a.isSame(p.first,p.second);
    });
    if(it!=db.end()){
      it->changebysalary(salary);
      return true;
    }
    return false;
  }


  bool setSalary(const string &email,
                 unsigned int salary)
  {
    sortbyEmail();
    vector<CEmployee>::iterator it = lower_bound(db.begin(),db.end(),email,[](const CEmployee & a, const string & email){
      return !a.isSame(email);
    });
    if(it!=db.end()){
      it->changebysalary(salary);
      return true;
    }
    return false;
  }


  // return the actual salary for the given employee.
  //  There are two variants of the method - the employee is identified either by the name and surname, or by the e-mail.
  //  If the methods succeed, they return the actual employee's salary.
  //  The return value is to be 0 to indicate a failure (employee not found).
  unsigned int getSalary(const string &name,
                         const string &surname) const
  {
    for(size_t i = 0 ; i < db.size() ; ++i)
    {
      if(db[i].isSame(name,surname))
      {
        return db[i].giveSalary();
      }
    }
    return 0;
  }

  unsigned int getSalary(const string &email) const
  {
    for(size_t i = 0 ; i < db.size() ; ++i)
    {
      if(db[i].isSame(email))
      {
        return db[i].giveSalary();
      }
    }
    return 0;
  }

  void sortbysalary() const
  {
    std::sort ( db.begin ( ), db.end ( ), [] ( const CEmployee & a, const CEmployee & b ) {
			return a.comparebySalary ( b );
		} );

  }

  void sortbyname()const
  {
    sort ( db.begin() , db.end() , [](const CEmployee & a , const CEmployee &b){
      return a.comparebyName(b);
    });
  }

  void sortbyEmail() const
  {
    sort(db.begin(),db.end(),[](const CEmployee & a, const CEmployee & b){
      return a.comparebyEmail(b);
    });
  }

  bool getRank(const string &name,
               const string &surname,
               int &rankMin,
               int &rankMax) const
  { 
    // sortEmployees();
    // bool change = false;
    // for(size_t i = 0 ; i < db.size() ; ++i)
    // {
    //   if(db[i].isSame(name,surname))
    //   { 
    //     if(! rankMin == i+1)
    //     {
    //       rankMin = i+1;
    //       change = true;
    //     }
    //   }
    // }
    // for(size_t i = rankMin ; i < db.size() ; ++i)
    // {
      
    // }
    // return false;
  }

  bool getRank(const string &email,
               int &rankMin,
               int &rankMax) const
  {
    // for(size_t i = 0 ; i < db.size() ; ++i)
    // {
    //   if(db[i].isSame(email))
    //   {
    //   }
    // }
    // return false;
  }
  bool getFirst(string &outName,
                string &outSurname) const;
  bool getNext(const string &name,
               const string &surname,
               string &outName,
               string &outSurname) const;
  
  void printEmployees() const
  {
    for(const CEmployee & employee : db)
    {
      cout << employee << endl;
    }
  }


private:
  // todo
  vector<CEmployee> db;

  template <typename T>
  bool removeEmployee(T type)
  {
    for(size_t i = 0 ; i < db.size() ; ++i)
    {
      if(type(db[i]))
      {
        db[i] = db.back();
        db.pop_back();
        return true;
      }
    }
    return false;
  }

};



#ifndef __PROGTEST__
int main(void)
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert(b1.add("John", "Smith", "john", 30000));
  assert(b1.add("John", "Miller", "johnm", 35000));
  assert(b1.add("Peter", "Smith", "peter", 23000));
  // assert ( b1 . getFirst ( outName, outSurname )
  //          && outName == "John"
  //          && outSurname == "Miller" );
  // assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
  //          && outName == "John"
  //          && outSurname == "Smith" );
  // assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
  //          && outName == "Peter"
  //          && outSurname == "Smith" );
  // assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
  // assert ( b1 . setSalary ( "john", 32000 ) );
  // assert ( b1 . getSalary ( "john" ) ==  32000 );
  // assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
  // assert ( b1 . getRank ( "John", "Smith", lo, hi )
  //          && lo == 1
  //          && hi == 1 );
  // assert ( b1 . getRank ( "john", lo, hi )
  //          && lo == 1
  //          && hi == 1 );
  // assert ( b1 . getRank ( "peter", lo, hi )
  //          && lo == 0
  //          && hi == 0 );
  // assert ( b1 . getRank ( "johnm", lo, hi )
  //          && lo == 2
  //          && hi == 2 );
  // assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  // assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  // assert ( b1 . getSalary ( "john" ) ==  35000 );
  // b1.printEmployees();
  // assert ( b1 . getRank ( "John", "Smith", lo, hi )
  //          && lo == 1
  //          && hi == 2 );
  // assert ( b1 . getRank ( "john", lo, hi )
  //          && lo == 1
  //          && hi == 2 );
  // assert ( b1 . getRank ( "peter", lo, hi )
  //          && lo == 0
  //          && hi == 0 );
  // assert ( b1 . getRank ( "johnm", lo, hi )
  //          && lo == 1
  //          && hi == 2 );
  assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  b1.printEmployees();
  // assert ( b1 . getSalary ( "peter" ) ==  23000 );
  // assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  // assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  // assert ( b1 . getFirst ( outName, outSurname )
  //          && outName == "James"
  //          && outSurname == "Bond" );
  // assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
  //          && outName == "John"
  //          && outSurname == "Miller" );
  // assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
  //          && outName == "John"
  //          && outSurname == "Smith" );
  // assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  // assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  // assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  // assert ( b1 . getSalary ( "james" ) ==  23000 );
  // assert ( b1 . getSalary ( "peter" ) ==  0 );
  assert ( b1 . del ( "james" ) );
  // assert ( b1 . getRank ( "john", lo, hi )
  //          && lo == 0
  //          && hi == 1 );
  assert ( b1 . del ( "John", "Miller" ) );
  // assert ( b1 . getRank ( "john", lo, hi )
  //          && lo == 0
  //          && hi == 0 );
  // assert ( b1 . getFirst ( outName, outSurname )
  //          && outName == "John"
  //          && outSurname == "Smith" );
  // assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . del ( "john" ) );
  // assert ( ! b1 . getFirst ( outName, outSurname ) );
  assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  // CPersonalAgenda b2;
  // assert ( ! b2 . getFirst ( outName, outSurname ) );
  // assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  // assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  // assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  // assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  // assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  // assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  // assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  // assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  // assert ( ! b2 . setSalary ( "joe", 90000 ) );
  // assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  // assert ( b2 . getSalary ( "joe" ) ==  0 );
  // assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  // assert ( ! b2 . getRank ( "joe", lo, hi ) );
  // assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  // assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  // assert ( ! b2 . del ( "Joe", "Black" ) );
  // assert ( ! b2 . del ( "joe" ) );
  // assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  // assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  // assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  // assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  // assert ( b2 . del ( "Peter", "Smith" ) );
  // assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  // assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  // assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  // assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  // assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  // assert ( ! b2 . setSalary ( "peter", 37000 ) );
  // assert ( b2 . getSalary ( "peter" ) ==  0 );
  // assert ( ! b2 . getRank ( "peter", lo, hi ) );
  // assert ( ! b2 . del ( "Peter", "Smith" ) );
  // assert ( ! b2 . del ( "peter" ) );
  // assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  // assert ( b2 . getSalary ( "peter" ) ==  40000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
