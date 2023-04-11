#include <iostream>
#include <iomanip>
using namespace std;


int main ( int argc, char * argv [] )
{
  int    a = 10;
  double b = 20;
  const char * c = "test";

  // width 50 chars, left padded (default - space)
  cout << "a = " << setw ( 50 ) << a << endl;

  // width 50 chars, left padded with '*'
  cout << "a = " << setw ( 50 ) << setfill ( '*' ) << a << endl;

  // width 20 chars, left padded with ' ', semilogarithmic format, 3 digits of mantissa
  cout << "b = " << setw ( 20 ) << setfill ( ' ' ) << setprecision ( 3 ) << b << endl;

  // width 20 chars, left padded with ' ', 3 fixed decimal places format
  cout << fixed;
  cout << "b = " << setw ( 20 ) << setfill ( ' ' ) << setprecision ( 3 ) << b << endl;

  cout << "c = " << c << endl;

  // various radix
  cout << "a = " << hex << a << " (hex) = " << oct << a << " (oct)"  << endl;
  // back to decimals
  cout << dec;

  // align to left (pad right) and the opposite
  cout << "c = " << setw ( 50 ) << left << c << endl;
  cout << "c = " << setw ( 50 ) << right << c << endl;

  return 0;
}

