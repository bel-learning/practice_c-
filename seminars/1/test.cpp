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

bool validateNum(string num) {
  if(num.size() != 9)
    return false;
  for(size_t i = 0; i < num.size(); i++) {
    if(!isdigit(num[i])) return false;
  }
  if(num[0] == '0') return false;
  return true;
}

struct Contact {
  string name;
  string surname;
  string number;
};

bool report ( const string & fileName, ostream & out )
{
  // TODO
  ifstream file(fileName, ios::in);

  string text;

  vector<Contact> contacts;

  if(file.fail()) return false;
  bool queries = false;
  while(getline(file, text)) {
    if(text.size() == 0 ) {
      queries = true;
      break;
    }
    stringstream ss(text);
    string name, surname, number;
    if ( ! (ss >> name >> surname >> number) ) {
      // cout << "after >> \n";
      return false;
    };
    string something;
    ss >> something;
    if(something.size() != 0) return false;
    if (!ss.eof()) {return false;};
    // cout << "before nothing\n";
    if(!validateNum(number)) return false;
    // cout << name << endl << surname << endl << number << endl;
    Contact tmp;
    tmp.name = name;
    tmp.surname = surname;
    tmp.number = number;
    contacts.push_back(tmp);
  }
  if(!queries) return false;

  if(file.eof()) return false;

  
  while(getline(file, text)) {
    string query;
    stringstream ss(text);

    if ( ! (ss >> query) ) { return false; };
    // cout << "query: " << query << endl;
    int matching = 0;

    for(size_t i = 0; i < contacts.size(); i++) {
      if(!query.compare(contacts[i].name) || !query.compare(contacts[i].surname)) {
        // cout << "matched!!!\n";
        // cout << contacts[i].name << " " << contacts[i].surname << " " << contacts[i].number << "\n";
        out << contacts[i].name << " " << contacts[i].surname << " " << contacts[i].number << "\n";
        matching++; 
      }
    }

    out << "-> " << matching << endl;
  }

  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
