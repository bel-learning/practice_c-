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
#include <iterator>
using namespace std;
#endif /* __PROGTEST__ */

class CPersonalAgenda
{
public:
    CPersonalAgenda(){}

    ~CPersonalAgenda(){}

    bool          add(const string& name, const string& surname, const string& email, unsigned int salary) {
        for (const auto& employee : employees) {
            if ((employee.name == name && employee.surname == surname) || employee.email == email) {
                return false;
                cout << "false at add";
            }
        }

        //Create new object to add to vector
        Employee* emp = new Employee();
        emp->name = name;
        emp->surname = surname;
        emp->email = email;
        emp->salary = salary;

        //Add new object to vector
        employees.push_back(*emp);

        //cout << employees[0].salary;

        return true;
    }


    bool          del(const string& name, const string& surname) {
        auto it = find_if(employees.begin(), employees.end(), [&](const Employee& employee) { return employee.name == name && employee.surname == surname; });

        if (it == employees.end()) {
            return false;
        }

        employees.erase(it);
        return true;
    }

    bool          del(const string& email) {
        auto it = find_if(employees.begin(), employees.end(), [&](const Employee& employee) { 
            //cout << "yes";
            return employee.email == email; });

        if (it == employees.end()) {
            return false;
        }

        employees.erase(it);
     
        return true;
    }

    bool          changeName(const string& email, const string& newName, const string& newSurname) {
        auto it = find_if(employees.begin(), employees.end(), [&](const Employee& employee) { return employee.email == email; });

        if (it == employees.end()) {
            return false;
        }
        for (const auto& employee : employees) {
            if (employee.email != email && employee.name == newName && employee.surname == newSurname) {
                return false;
            }
        }

        if (it->name == newName && it->surname == newSurname)
            return false;

        it->name = newName;
        it->surname = newSurname;
        return true;
    }

    bool          changeEmail(const string& name, const string& surname, const string& newEmail) {
        auto it = find_if(employees.begin(), employees.end(), [&](const Employee& employee) { return employee.name == name && employee.surname == surname; });

        if (it == employees.end()) {
            return false;
        }

        // Check if the new email is unique
        for (const auto& employee : employees) {
            if (employee.email == newEmail) {
                return false;
            }
        }

        it->email = newEmail;
        return true;
    }

    bool          setSalary(const string& name, const string& surname, unsigned int salary) {
        auto it = find_if(employees.begin(), employees.end(), [&](const Employee& employee) { return employee.name == name && employee.surname == surname; });

        if (it == employees.end()) {
            return false;
        }

        it->salary = salary;

        return true;
    }

    bool          setSalary(const string& email, unsigned int salary) {
        
        for (int i = 0; i < employees.size(); i++)
            if (employees[i].email == email)
            {
                employees[i].salary = salary;
                return true;
            }

        return false;
    }
    unsigned int  getSalary(const string& name, const string& surname) {

        int salary = 0;


        for (int i = 0; i < employees.size(); i++)
            if (employees[i].name == name && employees[i].surname == surname)
            {
                salary = employees[i].salary;
            }

        return salary;
    }

    unsigned int  getSalary(const string& email) {

        int salary=0;


        for (int i = 0; i < employees.size(); i++)
            if (employees[i].email == email)
                salary = employees[i].salary;

        return salary;
    }

    bool          getRank(const string& name, const string& surname, int& rankMin, int& rankMax) {

        int salary = 0;

        if (employees.size() == 0)
            return false;
        else
        {

            for (int i = 0; i < employees.size(); i++)
                if (employees[i].name == name && employees[i].surname == surname)
                {
                    salary = employees[i].salary;
                }

            if (salary != 0)
            {
                vector<int> Salaries;

                for (int i = 0; i < employees.size(); i++)
                {
                    Salaries.push_back(employees[i].salary);
                }

                sort(Salaries.begin(), Salaries.end());
                int jmax = -1;

                for (int i = 0; i < Salaries.size(); i++)
                    if (Salaries[i] == salary)
                    {
                        if (jmax == -1)
                            rankMin = i;

                        rankMax = i;
                        jmax++;
                    }

                return true;
            }
            else
                return false;
        }
    }

    bool          getRank(const string& email, int& rankMin, int& rankMax) {

        int salary = 0;

        if (employees.size() == 0)
            return false;
        else
        {

            for (int i = 0; i < employees.size(); i++)
                if (employees[i].email == email)
                {
                    salary = employees[i].salary;
                }

            if (salary != 0)
            {
                vector<int> Salaries;

                for (int i = 0; i < employees.size(); i++)
                {
                    Salaries.push_back(employees[i].salary);
                }

                sort(Salaries.begin(), Salaries.end());
                int jmax = -1;

                for (int i = 0; i < Salaries.size(); i++)
                    if (Salaries[i] == salary)
                    {
                        if (jmax == -1)
                            rankMin = i;

                        rankMax = i;
                        jmax++;
                    }

                return true;
            }
            else
                return false;
        }
    }


    bool          getFirst(string& outName, string& outSurname) {

        if (employees.size() == 0)
            return false;

        bubble_sort_name(employees);

        outName = employees[0].name;
        outSurname = employees[0].surname;

        return true;
    }

    bool          getNext(const string& name, const string& surname, string& outName, string& outSurname) {

        int i, j;

        for (i = 0; i < employees.size(); i++)
            if (employees[i].name == name && employees[i].surname == surname)
            {
                j = 1 + i;

                if (employees.size() == j)
                    return false;
                
                if (employees.size() > j)
                {
                    outName = employees[i + 1].name;
                    outSurname = employees[i + 1].surname;
           
                    return true;
                }
            }
    }


private:
    // todo

    struct Employee {
        string name, surname, email;
        int salary;
    };

    vector<Employee> employees;

    void bubble_sort_name(vector<Employee>& employees) {

        int n = employees.size();
        bool swapped;

        for (int i = 0; i < n - 1; i++) {
            swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                if (employees[j].surname > employees[j + 1].surname) {
                    std::swap(employees[j], employees[j + 1]);
                    swapped = true;
                }
                if (employees[j].surname == employees[j + 1].surname) {
                    if (employees[j].name > employees[j + 1].name) {
                        std::swap(employees[j], employees[j + 1]);
                        swapped = true;
                    }
                }
            }
            if (!swapped) break;
        }
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
    assert(b1.getFirst(outName, outSurname)
       && outName == "John"
       && outSurname == "Miller");
   assert(b1.getNext("John", "Miller", outName, outSurname)
        && outName == "John"
        && outSurname == "Smith");
    assert(b1.getNext("John", "Smith", outName, outSurname)
        && outName == "Peter"
        && outSurname == "Smith");
    assert(!b1.getNext("Peter", "Smith", outName, outSurname));
    assert(b1.setSalary("john", 32000));
    assert(b1.getSalary("john") == 32000);
    assert(b1.getSalary("John", "Smith") == 32000);
    assert(b1.getRank("John", "Smith", lo, hi)
        && lo == 1
        && hi == 1);
    assert(b1.getRank("john", lo, hi)
        && lo == 1
        && hi == 1);
    assert(b1.getRank("peter", lo, hi)
        && lo == 0
        && hi == 0);
    assert(b1.getRank("johnm", lo, hi)
        && lo == 2
        && hi == 2);
    assert(b1.setSalary("John", "Smith", 35000));
    assert(b1.getSalary("John", "Smith") == 35000);
    assert(b1.getSalary("john") == 35000);
    assert(b1.getRank("John", "Smith", lo, hi)
        && lo == 1
        && hi == 2);
    assert(b1.getRank("john", lo, hi)
        && lo == 1
        && hi == 2);
    assert(b1.getRank("peter", lo, hi)
        && lo == 0
        && hi == 0);
    assert(b1.getRank("johnm", lo, hi)
        && lo == 1
        && hi == 2);
    assert(b1.changeName("peter", "James", "Bond"));
    assert(b1.getSalary("peter") == 23000);
    assert(b1.getSalary("James", "Bond"));
    assert(b1.getSalary("Peter", "Smith") == 0);
    assert(b1.getFirst(outName, outSurname)
        && outName == "James"
        && outSurname == "Bond");
    assert(b1.getNext("James", "Bond", outName, outSurname)
        && outName == "John"
        && outSurname == "Miller");
    assert(b1.getNext("John", "Miller", outName, outSurname)
        && outName == "John"
        && outSurname == "Smith");
    assert(!b1.getNext("John", "Smith", outName, outSurname));
    assert(b1.changeEmail("James", "Bond", "james"));
    assert(b1.getSalary("James", "Bond") == 23000);
    assert(b1.getSalary("james") == 23000);
    assert(b1.getSalary("peter") == 0);
    assert(b1.del("james"));
    assert(b1.getRank("john", lo, hi)
        && lo == 0
        && hi == 1);
    assert(b1.del("John", "Miller"));
    assert(b1.getRank("john", lo, hi)
        && lo == 0
        && hi == 0);
    assert(b1.getFirst(outName, outSurname)
        && outName == "John"
        && outSurname == "Smith");
    assert(!b1.getNext("John", "Smith", outName, outSurname));
    assert(b1.del("john"));
    assert(!b1.getFirst(outName, outSurname));
    assert(b1.add("John", "Smith", "john", 31000));
    assert(b1.add("john", "Smith", "joHn", 31000));
    assert(b1.add("John", "smith", "jOhn", 31000));

    CPersonalAgenda b2;
    assert(!b2.getFirst(outName, outSurname));
    assert(b2.add("James", "Bond", "james", 70000));
    assert(b2.add("James", "Smith", "james2", 30000));
    assert(b2.add("Peter", "Smith", "peter", 40000));
    assert(!b2.add("James", "Bond", "james3", 60000));
    assert(!b2.add("Peter", "Bond", "peter", 50000));
    assert(!b2.changeName("joe", "Joe", "Black"));
    assert(!b2.changeEmail("Joe", "Black", "joe"));
    assert(!b2.setSalary("Joe", "Black", 90000));
    assert(!b2.setSalary("joe", 90000));
    assert(b2.getSalary("Joe", "Black") == 0);
    assert(b2.getSalary("joe") == 0);
    assert(!b2.getRank("Joe", "Black", lo, hi));
    assert(!b2.getRank("joe", lo, hi));
    assert(!b2.changeName("joe", "Joe", "Black"));
    assert(!b2.changeEmail("Joe", "Black", "joe"));
    assert(!b2.del("Joe", "Black"));
    assert(!b2.del("joe"));
    assert(!b2.changeName("james2", "James", "Bond"));
    assert(!b2.changeEmail("Peter", "Smith", "james"));
    assert(!b2.changeName("peter", "Peter", "Smith"));
    assert(!b2.changeEmail("Peter", "Smith", "peter"));
    assert(b2.del("Peter", "Smith"));
    assert(!b2.changeEmail("Peter", "Smith", "peter2"));
    assert(!b2.setSalary("Peter", "Smith", 35000));
    assert(b2.getSalary("Peter", "Smith") == 0);
    assert(!b2.getRank("Peter", "Smith", lo, hi));
    assert(!b2.changeName("peter", "Peter", "Falcon"));
    assert(!b2.setSalary("peter", 37000));
    assert(b2.getSalary("peter") == 0);
    assert(!b2.getRank("peter", lo, hi));
    assert(!b2.del("Peter", "Smith"));
    assert(!b2.del("peter"));
    assert(b2.add("Peter", "Smith", "peter", 40000));
    assert(b2.getSalary("peter") == 40000);

   return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

