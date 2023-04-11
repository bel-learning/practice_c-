#include <iostream>
#include <iomanip>
using namespace std;

/* Read input integers, sum them into an accumulator. Stop reading on EOF.
 * Sanitize input, report an error on invalid input data.
 */

int main ( int argc, char * argv [] )
{
  int x, sum = 0;

  cout << "Enter a sequence of positive integers, EOF terminates:" << endl;
  while ( cin >> x && x > 0 )
    sum += x; // read a positive integer - accumulate

  if ( ! cin . eof () ) // EOF not reached -> something wrong was in the input.
  {
    cout << "Invalid input." << endl;
    return 1;
  }

  cout << "Sum = " << sum << endl;
  return 0;
}
