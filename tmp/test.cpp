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
int compareByEmailInt(const Employee * a, const Employee * b) {
  return a->email.compare(b->email);
}
int compareByNameInt(const Employee * a, const Employee * b) {
  int state = a->surname.compare(b->surname);
  if(state == 0) 
    return a->name.compare(b->name);
  return state;
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
int compareBySalaryInt(const Employee * a, const Employee * b) {
  return (a->salary - b->salary);
}
bool compareBySalary(const Employee * a, const Employee * b) {
  return a->salary < b->salary;
}

class CPersonalAgenda
{
  public:
                  CPersonalAgenda  ( void ) {
                    emp_email_sorted.reserve(100);
                    emp_name_sorted.reserve(100);
                    emp_salary_sorted.reserve(100);
                  };
                  ~CPersonalAgenda ( void ) {
                    for(size_t i = 0; i < emp_name_sorted.size(); i++) {
                      delete emp_name_sorted[i];
                    }
                  };
    void printArrays() const {
      cout << "-------------" << endl;
      cout << "Emp name sorted" << endl;
      for(size_t i = 0; i < emp_name_sorted.size(); i++) {
        cout << emp_name_sorted[i]->surname << " - "  << emp_name_sorted[i]->name<< " - " << emp_name_sorted[i]->salary << " @ " << emp_name_sorted[i]->email <<endl;
      }
      cout << "Emp email sorted" << endl;
      for(size_t i = 0; i < emp_email_sorted.size(); i++) {
        cout << emp_email_sorted[i]->email << " - "<< emp_email_sorted[i]->salary << endl;
      }
      cout << "Emp salary sorted" << endl;
      for(size_t i = 0; i < emp_salary_sorted.size(); i++) {
        cout << emp_salary_sorted[i]->salary << endl;
      }
      cout << "-------------" << endl;

    }
    bool          add              ( const string    & name,
                                     const string    & surname,
                                     const string    & email,
                                     unsigned int      salary ) 
    {
      Employee * emp = new Employee;
      *emp = {name, surname, email, salary};

      if(employeeEmailExists(emp)) {delete emp; return false;};
      if(employeeNameExists(emp)) {delete emp; return false;};

      pushToSortedArray(emp, emp_email_sorted, compareByEmailInt);
      pushToSortedArray(emp, emp_name_sorted, compareByNameInt);
      pushToSortedArray(emp, emp_salary_sorted, compareBySalaryInt);
      resize();
      return true;
    };
    bool          del              ( const string    & name,
                                     const string    & surname )
    {
      Employee emp;
      emp.surname = surname;
      emp.name = name;
      if(!employeeNameExists(&emp)) return false;

      int index = searchSortedArray(&emp, emp_name_sorted, compareByNameInt);
      if(index == -1) 
        return false;
      Employee * tmp_emp = emp_name_sorted[index];
      
      tmp_emp = emp_name_sorted[index];
      emp_name_sorted.erase(emp_name_sorted.begin() +index);
      index = searchSortedArray(tmp_emp, emp_email_sorted, compareByEmailInt);

      emp_email_sorted.erase(emp_email_sorted.begin() + index);

      index = searchBySalary(tmp_emp);
      emp_salary_sorted.erase(emp_salary_sorted.begin() + index);
      
      delete tmp_emp;
      return true;
    };
    bool          del              ( const string    & email )
    {
      Employee emp;
      emp.email = email;

      int index = searchSortedArray(&emp, emp_email_sorted, compareByEmailInt);
      if(index == -1) 
        return false;
      Employee * tmp_emp = emp_email_sorted[index];
      
      emp_email_sorted.erase(emp_email_sorted.begin() +index);
      index = searchSortedArray(tmp_emp, emp_name_sorted, compareByNameInt);

      emp_name_sorted.erase(emp_name_sorted.begin() + index);

      index = searchBySalary(tmp_emp);
      emp_salary_sorted.erase(emp_salary_sorted.begin() + index);

      delete tmp_emp;
      return true;
    };
    bool          changeName       ( const string    & email,
                                     const string    & newName,
                                     const string    & newSurname )
    {
       Employee tmp;

      tmp.email = email;
      tmp.name = newName;
      tmp.surname = newSurname;
      if(employeeNameExists(&tmp)) return false;
      if(emp_email_sorted.size() == 0) return false;
      
      int index = searchSortedArray(&tmp, emp_email_sorted, compareByEmailInt);
      if(index == -1) return false;
      
      Employee * emp = emp_email_sorted[index];

      int remove_index = searchSortedArray(emp, emp_name_sorted, compareByNameInt);
      emp_name_sorted.erase(emp_name_sorted.begin() + remove_index);
      
      emp->name.assign(newName);
      emp->surname.assign(newSurname);

      int insert_index = whereToPut(emp, emp_name_sorted, compareByNameInt);
      emp_name_sorted.insert(emp_name_sorted.begin() + insert_index, emp);

      return true; 
    };
    bool          changeEmail      ( const string    & name,
                                     const string    & surname,
                                     const string    & newEmail )
    {
      Employee tmp;
      tmp.name = name;
      tmp.surname = surname;
      tmp.email = newEmail;
      // cout << name << " " << surname << " change his email to " << newEmail << endl;
      // searching for new email
      if(employeeEmailExists(&tmp)) return false;
      if(emp_name_sorted.size() == 0) return false;

      int index = searchSortedArray(&tmp, emp_name_sorted, compareByNameInt);
      if(index == -1) return false;
      
      Employee * emp = emp_name_sorted[index];

      
      int remove_index = searchSortedArray(emp, emp_email_sorted, compareByEmailInt);
      emp_email_sorted.erase(emp_email_sorted.begin() + remove_index);
      
      emp->email.assign(newEmail);

      int insert_index = whereToPut(emp, emp_email_sorted, compareByEmailInt);
      emp_email_sorted.insert(emp_email_sorted.begin() + insert_index, emp);
      // printArrays();
      return true;
    };
    bool          setSalary        ( const string    & name,
                                     const string    & surname,
                                     unsigned int      salary )
    {
      Employee tmp;
      tmp.name = name;
      tmp.surname = surname;
      int index = searchSortedArray(&tmp, emp_name_sorted, compareByNameInt);
      if(index == -1) return false;
      Employee * emp = emp_name_sorted[index];
      int remove_index = searchBySalary(emp);
      emp_salary_sorted.erase(emp_salary_sorted.begin() + remove_index);
      emp->salary = salary;
      int insert_index = whereToPut(emp, emp_salary_sorted, compareBySalaryInt);
      emp_salary_sorted.insert(emp_salary_sorted.begin() + insert_index, emp);

      return true;
    };
    bool          setSalary        ( const string    & email,
                                     unsigned int      salary )
    {
      Employee tmp;
      tmp.email = email;
      int index = searchSortedArray(&tmp, emp_email_sorted, compareByEmailInt);
      if(index == -1) return false;
      Employee * emp = emp_email_sorted[index];
      int remove_index = searchBySalary(emp);
      emp_salary_sorted.erase(emp_salary_sorted.begin() + remove_index);
      emp->salary = salary;
      int insert_index = whereToPut(emp, emp_salary_sorted, compareBySalaryInt);
      emp_salary_sorted.insert(emp_salary_sorted.begin() + insert_index, emp);
      
      return true;
    };
    unsigned int  getSalary        ( const string    & name,
                                     const string    & surname ) const
    {
      Employee tmp;
      tmp.name = name;
      tmp.surname = surname;

      int index = searchSortedArray(&tmp, emp_name_sorted, compareByNameInt);
      if(index == -1) return 0;

      return emp_name_sorted[index]->salary;
    };
    unsigned int  getSalary        ( const string    & email ) const
    {
      Employee tmp;
      tmp.email = email;
      int index = searchSortedArray(&tmp, emp_email_sorted, compareByEmailInt);
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
      int indexOfEmp = searchSortedArray(&tmp, emp_name_sorted, compareByNameInt);
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
      int indexOfEmp = searchSortedArray(&tmp, emp_email_sorted, compareByEmailInt);
      if(indexOfEmp == -1) return false;
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
      Employee tmp_emp;
      tmp_emp.name = name;
      tmp_emp.surname = surname;
      int lower;
      lower = searchSortedArray(&tmp_emp, emp_name_sorted, compareByNameInt);
      if(lower == -1) return false;
      if(lower + 1 >= (int) emp_name_sorted.size()) return false;
      outName = emp_name_sorted[lower+1]->name;
      outSurname = emp_name_sorted[lower+1]->surname;
      return true;
    };

  private:
    // todo
    vector<Employee *> emp_name_sorted;
    vector<Employee *> emp_email_sorted;
    vector<Employee *> emp_salary_sorted;

    
    void resize() {
      if(emp_name_sorted.size()+1 >= emp_name_sorted.capacity()) {
        emp_name_sorted.reserve(emp_name_sorted.capacity() * 2);
        emp_email_sorted.reserve(emp_email_sorted.capacity() * 2);
        emp_salary_sorted.reserve(emp_salary_sorted.capacity() * 2);
      }
    }

    int linearSearchArray(Employee * emp, vector<Employee *> & sortedArr, bool (*comp)(const Employee *,const Employee *)) {
      for(size_t i = 0; i < sortedArr.size(); i++) {
        
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
    int whereToPut(const Employee * emp, const vector<Employee *> & sortedArr , int (*comp)(const Employee *,const Employee *)) {
      int lower = 0;
      int upper = sortedArr.size() - 1;
      while (lower <= upper) {
          int mid = lower + (upper - lower) / 2;
          int res = comp(emp, sortedArr[mid]);
          if (res == 0)
            return mid;
          if (res > 0)
            lower = mid + 1;
          else
            upper = mid - 1;
      }
      return upper+1;
    }

    int searchSortedArray(const Employee * emp, const vector<Employee *> & sortedArr , int (*comp)(const Employee *,const Employee *)) const {
      if(sortedArr.size() == 0) return -1;
      int lower = 0;
      int upper = sortedArr.size() - 1;
      while (lower <= upper) {
          int mid = lower + (upper - lower) / 2;
          int res = comp(emp, sortedArr[mid]);
          if (res == 0)
            return mid;
          if (res > 0)
            lower = mid + 1;
          else
            upper = mid - 1;
      }
      return -1;
    }
    

    int getLowerBound(Employee * emp, const vector<Employee *> & sortedArr , bool (*comp)(const Employee *,const Employee *)) const {
      vector<Employee *>::const_iterator lower;
      lower = lower_bound(sortedArr.begin(), sortedArr.end(), emp, comp);
      return (lower - sortedArr.begin());
    }
    int getUpperBound(Employee * emp,const vector<Employee *> & sortedArr, bool (*comp)(const Employee *,const Employee *)) const {
      vector<Employee *>::const_iterator upper;
      upper = upper_bound(sortedArr.begin(), sortedArr.end(), emp, comp);
      return (upper - sortedArr.begin());
    }
    bool employeeEmailExists(const Employee * emp) const {
      return binary_search(emp_email_sorted.begin(),emp_email_sorted.end(), emp, compareByEmail);
    }
    
    bool employeeNameExists(const Employee * emp) const {
      return binary_search(emp_name_sorted.begin(),emp_name_sorted.end(), emp, compareByName);
    }
   
    void pushToSortedArray(Employee * emp, vector<Employee *> & sortedArr, int (*comp)(const Employee *,const Employee *)) {
      int index = whereToPut(emp, sortedArr, comp);
      sortedArr.insert(sortedArr.begin() + index, emp);
    } 
    int searchBySalary(Employee * emp) const {
      vector<Employee *>::const_iterator lower, upper;
      lower = lower_bound(emp_salary_sorted.begin(), emp_salary_sorted.end(), emp, compareBySalary);
      upper = upper_bound(emp_salary_sorted.begin(), emp_salary_sorted.end(), emp, compareBySalary);
      for(auto it = lower; it != upper; it++) {
        if(empEqual(emp, (*it))) {
          return (it - emp_salary_sorted.begin());
        }
      }
      return -1;
    }
    bool empEqual(Employee * emp1, Employee * emp2) const {
      if(emp1->email != emp2->email) return false;
      if(emp1->surname != emp2->surname) return false;
      if(emp1->name != emp2->name) return false;
      if(emp1->salary != emp2->salary) return false;
      return true;
    }

};

#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  // assert ( b1 . add ( "John", "AB", "john", 30000 ) );
  // assert ( b1 . add ( "John", "BA", "johnm", 35000 ) );
  // assert ( b1 . add ( "Peter", "CA", "peter", 23000 ) );
  // assert ( b1.changeName ( "peter", "Whatever", "AC" ) );
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
  assert ( b1.getSalary("John", "Smith"));
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
  // b1.printArrays();
  // b1.printArrays();

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
  // b2.printArrays();
  assert (b2.setSalary("james2", 100000));
  // b2.printArrays();


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
