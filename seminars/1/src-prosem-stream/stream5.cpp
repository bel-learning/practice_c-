#include <climits>  // INT_MAX
#include <cmath>    // sqrt, atan2
#include <iostream>
#include <iomanip>
#include <sstream>  // istringstream

using namespace std;

/* read complex numbers (real and imaginary component). The input shall be of the form:
 * (re, im)
 * Moreover, the program shall detect there is exactly one coordinate on a line.
 */

/* this function gets C++ string (string data type) and parses it. If ok, return true and
 * output parameters x,y are set accordingly. If the input format is invalid, the
 * function returns false
 */
bool parseCoord ( string s, double & x, double & y )
{
  istringstream is ( s );       // we need to parse the string (memory buffer).
                                // istreingstream has the functionality of istream,
                                // it reads the bytes from the buffer (the streing s here)
  char dummy1, dummy2, dummy3;

  if ( ! ( is >> dummy1 >> x >> dummy2 >> y >> dummy3 ) // try to read the coordinate
       || dummy1 != '('
       || dummy2 != ','
       || dummy3 != ')' )
    return false;

  // The reading of dummy3 has succeeded -> is does not report eof (yet).
  // Try to read one more character. If there is something else on the input line, the next reading
  // will not set eof -> we know the input is not well formatted. If the reading sets eof -> format is ok
  // Note that the eof flag is related to out istringstream instance, not to the standard input.

  // Our current setup does not allow whitespace after the closing parenthesis.
  // If we add:
  // is >> ws;
  // we would skip the trailing whitespace

  is . get ();  // try to read one more character
  return is . eof ();  // ok if eof
}

int main ( int argc, char * argv [] )
{
  double  x, y;
  string  line;

  cout << "Enter coordinates in the form (x,y):" << endl;
  while ( getline ( cin, line ) // read one input line, place it to the string
          && parseCoord ( line, x, y ) ) // if parsing fails - cancel the loop
    cout << '(' << x << ',' << y << ") = (" <<
         sqrt ( x * x + y * y ) << '<' << atan2 ( y, x ) << ')' << endl;

  if ( ! cin . eof () )
  {
    cout << "Invalid input." << endl;
    return 1;
  }
  return 0;
}

