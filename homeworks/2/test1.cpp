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

struct Employee
{
  string name;
  string surname;
  string email;
  unsigned int salary;
};
bool compareByEmail(const Employee * a, const Employee * b) {
  return a->email < b->email;
}
bool compareByName(const Employee * a, const Employee * b) {
  if(a->surname < b->surname) {
      return true;
    }
  else if(a->surname == b->surname) {
    return a->name < b->name;
  }
  else {
    return false;
  }
}
bool compareBySalary(const Employee * a, const Employee * b) {
  return a->salary < b->salary;
}


class CPersonalAgenda
{
  public:
                  CPersonalAgenda  ( void ) {};
                  ~CPersonalAgenda ( void ) {
                    for(size_t i = 0; i < emp_name_sorted.size(); i++) {
                      delete emp_name_sorted[i];
                    }
                  };
    bool          add              ( const string    & name,
                                     const string    & surname,
                                     const string    & email,
                                     unsigned int      salary ) 
    {
      Employee * emp = new Employee;
      *emp = {name, surname, email, salary};

      if(employeeEmailExists(emp)) return false;
      if(employeeNameExists(emp)) return false;

      pushToSortedArray(emp, emp_email_sorted, compareByEmail);
      pushToSortedArray(emp, emp_name_sorted, compareByName);
      pushToSortedArray(emp, emp_salary_sorted, compareBySalary);


      return true;
    };
    bool          del              ( const string    & name,
                                     const string    & surname )
    {
      Employee emp;
      emp.surname = surname;
      emp.name = name;
      if(!employeeNameExists(&emp)) return false;

      int index = searchSortedArray(&emp, emp_name_sorted, compareByName);
      Employee * tmp_emp = emp_name_sorted[index];

      if(index == -1) 
        return false;
      else {
        tmp_emp = emp_name_sorted[index];
        emp_name_sorted.erase(emp_name_sorted.begin() +index);
        index = searchSortedArray(tmp_emp, emp_email_sorted, compareByEmail);

        // if(index == -1) { cout << "-1 index" << endl;}
        emp_email_sorted.erase(emp_email_sorted.begin() + index);

        index = searchSortedArray(tmp_emp, emp_salary_sorted, compareBySalary);

        // if(index == -1) { cout << "-1 index" << endl;}
        emp_salary_sorted.erase(emp_salary_sorted.begin() + index);
      }


      delete tmp_emp;
      return true;
    };
    bool          del              ( const string    & email )
    {
      Employee emp;
      emp.email = email;
      if(!employeeEmailExists(&emp)) return false;

      int index = searchSortedArray(&emp, emp_email_sorted, compareByEmail);

      Employee * tmp_emp = emp_email_sorted[index];

      if(index == -1) 
        return false;
      else {
        emp_email_sorted.erase(emp_email_sorted.begin() +index);
        index = searchSortedArray(tmp_emp, emp_name_sorted, compareByName);

        // if(index == -1) { cout << "-1 index" << endl;}
        emp_name_sorted.erase(emp_name_sorted.begin() + index);

        index = searchSortedArray(tmp_emp, emp_salary_sorted, compareBySalary);

        // if(index == -1) { cout << "-1 index" << endl;}
        emp_salary_sorted.erase(emp_salary_sorted.begin() + index);
      }
      

      delete tmp_emp;
      return true;
    };
    bool          changeName       ( const string    & email,
                                     const string    & newName,
                                     const string    & newSurname )
    {
       Employee tmp;
      tmp.email = email;
      if(!employeeEmailExists(&tmp)) return false;
      tmp.name = newName;
      tmp.surname = newSurname;
      if(employeeNameExists(&tmp)) return false;
      if(emp_email_sorted.size() == 0) return false;
      
      int index = searchSortedArray(&tmp, emp_email_sorted, compareByEmail);
      if(index == -1) return false;
      
      emp_email_sorted[index]->name.assign(newName);
      emp_email_sorted[index]->surname.assign(newSurname);
      // can be made faster;
      sort(emp_name_sorted.begin(), emp_name_sorted.end(), compareByName);
      return true;
    };
    bool          changeEmail      ( const string    & name,
                                     const string    & surname,
                                     const string    & newEmail )
    {
      Employee tmp;
      tmp.name = name;
      tmp.surname = surname;
      if(!employeeNameExists(&tmp)) return false;
      tmp.email = newEmail;
      if(employeeEmailExists(&tmp)) return false;

      if(emp_name_sorted.size() == 0) return false;
      int index = searchSortedArray(&tmp, emp_name_sorted, compareByName);
      if(index == -1) return false;
      
      emp_name_sorted[index]->email.assign(newEmail);
      sort(emp_email_sorted.begin(), emp_email_sorted.end(), compareByEmail);

      return true;
    };
    bool          setSalary        ( const string    & name,
                                     const string    & surname,
                                     unsigned int      salary )
    {
      Employee tmp;
      tmp.name = name;
      tmp.surname = surname;
      if(!employeeNameExists(&tmp)) return false;
      int index = searchSortedArray(&tmp, emp_name_sorted, compareByName);
      if(index == -1) return false;
      emp_name_sorted[index]->salary = salary;

      return true;
    };
    bool          setSalary        ( const string    & email,
                                     unsigned int      salary )
    {
      Employee tmp;
      tmp.email = email;
      if(!employeeEmailExists(&tmp)) return false;
      int index = searchSortedArray(&tmp, emp_email_sorted, compareByEmail);
      if(index == -1) return false;
      emp_email_sorted[index]->salary = salary;
      
      return true;
    };
    unsigned int  getSalary        ( const string    & name,
                                     const string    & surname ) const
    {
      Employee tmp;
      tmp.name = name;
      tmp.surname = surname;
      if(!employeeNameExists(&tmp)) return 0;

      int index = searchSortedArray(&tmp, emp_name_sorted, compareByName);
      if(index == -1) return 0;
      return emp_name_sorted[index]->salary;
    };
    unsigned int  getSalary        ( const string    & email ) const
    {
      Employee tmp;
      tmp.email = email;
      if(!employeeEmailExists(&tmp)) return 0;

      int index = searchSortedArray(&tmp, emp_email_sorted, compareByEmail);
      if(index == -1) return 0;
      return emp_email_sorted[index]->salary;
    };
    bool          getRank          ( const string    & name,
                                     const string    & surname,
                                     int             & rankMin,
                                     int             & rankMax ) const
    {
      Employee tmp;
      tmp.name = name;
      tmp.surname = surname;
      if(!employeeNameExists(&tmp)) return false;
      int indexOfEmp = searchSortedArray(&tmp, emp_name_sorted, compareByName);
      if(indexOfEmp == -1) return 0;
      Employee * emp = emp_name_sorted[indexOfEmp];
      int lowerBound = getLowerBound(emp, emp_salary_sorted, compareBySalary);
      int upperBound = getUpperBound(emp, emp_salary_sorted, compareBySalary);
      


      rankMin = lowerBound;
      rankMax = upperBound - 1;
      return true;
    };
    bool          getRank          ( const string    & email,
                                     int             & rankMin,
                                     int             & rankMax ) const
    {
      Employee tmp;
      tmp.email = email;
      if(!employeeEmailExists(&tmp)) return false;
      int indexOfEmp = searchSortedArray(&tmp, emp_email_sorted, compareByEmail);
      if(indexOfEmp == -1) return 0;
      Employee * emp = emp_email_sorted[indexOfEmp];
      int lowerBound = getLowerBound(emp, emp_salary_sorted, compareBySalary);
      int upperBound = getUpperBound(emp, emp_salary_sorted, compareBySalary);


      rankMin = lowerBound;
      rankMax = upperBound - 1;
      return true;
    };
    bool          getFirst         ( string          & outName,
                                     string          & outSurname ) const
    {
      if(emp_name_sorted.size() == 0) return false;
      outName = emp_name_sorted[0]->name;
      outSurname = emp_name_sorted[0]->surname;
      return true;
    };
    bool          getNext          ( const string    & name,
                                     const string    & surname,
                                     string          & outName,
                                     string          & outSurname ) const
    {
      if(emp_name_sorted.size() == 0) return false;
      Employee * tmp_emp = new Employee;
      tmp_emp->name = name;
      tmp_emp->surname = surname;

      int lower;
      lower = getLowerBound(tmp_emp, emp_name_sorted, compareByName);
      if(lower == -1) return false;
      if(lower + 1 >= emp_name_sorted.size()) return false;
      outName = emp_name_sorted[lower+1]->name;
      outSurname = emp_name_sorted[lower+1]->surname;
      return true;
    };

  private:
    // todo
    vector<Employee *> emp_name_sorted;
    vector<Employee *> emp_email_sorted;
    vector<Employee *> emp_salary_sorted;

    int linearSearchArray(Employee * emp, vector<Employee *> & sortedArr, bool (*comp)(const Employee *,const Employee *)) {
      vector<Employee *>::iterator it;
      for(int i = 0; i < sortedArr.size(); i++) {
        
        if(sortedArr[i]->email == emp->email &&
            sortedArr[i]->surname == emp->surname &&
            sortedArr[i]->name == emp->name &&
            sortedArr[i]->salary == emp->salary
          ) {
            return i;
          }
      }
      return -1;
    }

    int searchSortedArray(Employee * emp, vector<Employee *>  sortedArr, bool (*comp)(const Employee *,const Employee *)) const {
      vector<Employee *>::iterator lower;
      lower = lower_bound(sortedArr.begin(), sortedArr.end(), emp, comp);
      if(lower == sortedArr.end()) {
        return -1;
      }
      else {
        return lower - sortedArr.begin();
      }
    }

    int searchSortedArray(Employee * emp, vector<Employee *> & sortedArr, bool (*comp)(const Employee *,const Employee *)) {
      vector<Employee *>::iterator lower;
      lower = lower_bound(sortedArr.begin(), sortedArr.end(), emp, comp);
      if(lower == sortedArr.end()) {
        return -1;
      }
      else {
        return lower - sortedArr.begin();
      }
    }

    int getLowerBound(Employee * emp, vector<Employee *> sortedArr, bool (*comp)(const Employee *,const Employee *)) const {
      vector<Employee *>::iterator lower;
      lower = lower_bound(sortedArr.begin(), sortedArr.end(), emp, comp);
      return (lower - sortedArr.begin());
    }
    int getUpperBound(Employee * emp, vector<Employee *> sortedArr, bool (*comp)(const Employee *,const Employee *)) const {
      vector<Employee *>::iterator upper;
      upper = upper_bound(sortedArr.begin(), sortedArr.end(), emp, comp);
     
      return (upper - sortedArr.begin());
    }
    bool employeeEmailExists(const Employee * emp) const {
      return binary_search(emp_email_sorted.begin(),emp_email_sorted.end(), emp, compareByEmail);
    }
    
    bool employeeNameExists(const Employee * emp) const {
      return binary_search(emp_name_sorted.begin(),emp_name_sorted.end(), emp, compareByName);
    }
   
    void pushToSortedArray(Employee * emp, vector<Employee *> & sortedArr, bool (*comp)(const Employee *,const Employee *)) {
      vector<Employee *>::iterator lower;
      lower = lower_bound(sortedArr.begin(), sortedArr.end(), emp, comp);
      if(lower == sortedArr.end()) 
        sortedArr.push_back(emp);
      else
        sortedArr.insert(lower, emp);
    } 
    void printArrays() const {
      cout << "Emp name sorted" << endl;
      for(size_t i = 0; i < emp_name_sorted.size(); i++) {
        cout << emp_name_sorted[i]->surname << " - "  << emp_name_sorted[i]->name<< " - " << emp_email_sorted[i]->salary <<endl;
      }
      cout << "Emp email sorted" << endl;
      for(size_t i = 0; i < emp_email_sorted.size(); i++) {
        cout << emp_email_sorted[i]->email << " - "<< emp_email_sorted[i]->salary << endl;
      }
      cout << "Emp salary sorted" << endl;
      for(size_t i = 0; i < emp_salary_sorted.size(); i++) {
        cout << emp_salary_sorted[i]->salary << endl;
      }
    }
};

#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
  assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
  // assert(b1.del("John", "Smith"));
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . setSalary ( "john", 32000 ) );
  assert ( b1 . getSalary ( "john" ) ==  32000 );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . getSalary ( "john" ) ==  35000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  assert ( b1 . getSalary ( "peter" ) ==  23000 );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "james" ) ==  23000 );
  assert ( b1 . getSalary ( "peter" ) ==  0 );
  assert ( b1 . del ( "james" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . del ( "John", "Miller" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . del ( "john" ) );
  assert ( ! b1 . getFirst ( outName, outSurname ) );
  assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . getFirst ( outName, outSurname ) );
  assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . setSalary ( "joe", 90000 ) );
  assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . getSalary ( "joe" ) ==  0 );
  assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . getRank ( "joe", lo, hi ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . del ( "Joe", "Black" ) );
  assert ( ! b2 . del ( "joe" ) );
  assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . setSalary ( "peter", 37000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  0 );
  assert ( ! b2 . getRank ( "peter", lo, hi ) );
  assert ( ! b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . del ( "peter" ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  40000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
