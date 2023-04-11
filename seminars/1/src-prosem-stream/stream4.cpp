#include <climits>  // INT_MAX
#include <iostream>
#include <iomanip>
using namespace std;

/* Read a pair of integers, display the sum. If the input is invalid,
 * ask for a correct input.
 */

int main ( int argc, char * argv [] )
{
  int a, b;

  cout << "Enter two positive integers:" << endl;
  while ( ! ( cin >> a >> b ) // yes, we may read more variables at once
          || a <= 0
          || b <= 0 )
  {  // invalid input
    cin . clear ();                 // clear the fail bit
    cin . ignore ( INT_MAX, '\n' ); // discard the input (the invalid characters), until the end-of-line
    cout << "Invalid input. Please enter two positive integers:" << endl;
  }
  cout << "Sum: " << (a+b) << endl;
  return 0;
}

