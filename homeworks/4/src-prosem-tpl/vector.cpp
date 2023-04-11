#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

int main ( void )
{
  vector <int> x;
/*
  x[0] = 10;
  cout << x[0] <<endl;
*/

  x . push_back ( 10 );
  x . push_back ( 20 );
  x . push_back ( 13 );
  x . push_back ( 14 );

/*
  for ( vector<int>::size_type i = 0; i < x . size (); i ++ )
    cout << x[i] << endl;
*/
  int idx = 0;
  for_each ( x . begin (), x . end (), 
             [&idx] ( const int & a ) 
             { 
               cout << idx ++ << " " << a << endl; 
             } );

  return 0;
}