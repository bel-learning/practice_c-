#include <iostream>
#include <iomanip>
using namespace std;

/* Read input integers, sum them into an accumulator. Stop reading on EOF.
 * Sanitize input, report an error on invalid input data.
 *
 * The previous version has a tiny problem if the input consists of positive
 * numbers and a negative number at the end (without any newline or whitespace
 * following that negative number - it is a bit tricky to test, input
 * redirection must be used). In that case the last number is not included
 * into the sum, however, error is not reported. This version fixes the problem.
 */

int main ( int argc, char * argv [] )
{
  int x, sum = 0;
  bool readOk;

  cout << "Enter a sequence of positive integers, EOF terminates:" << endl;
  while ( ( readOk = (bool)(cin >> x) )
          && x > 0 )
    sum += x; // read a positive integer - accumulate

  // readOk is true - successfully read the number, yet quit the loop - negative
  // ! eof - data available in the input, but quit the loop - invalid format (not an int)
  if ( readOk || ! cin . eof () )
  {
    cout << "Invalid input." << endl;
    return 1;
  }

  cout << "Sum = " << sum << endl;
  return 0;
}
