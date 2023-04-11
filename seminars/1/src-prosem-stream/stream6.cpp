#include <iostream>
#include <iomanip>
#include <sstream>  // ostringstream
#include <fstream>  // fstream
using namespace std;

/* hexdump - display binary file in a human readable form (as hex numbers
 * representing bytes + character representation (for printable bytes)
 *
 * The expected format is
 * 00000000   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 cccccccccccccccc
 * address     16 bytes (as hex)                              16 bytes (as char)
 */


bool dumpFile ( const char * fileName )
{
  // open file in binary format. We need to see every byte without any library conversions
  ifstream      f ( fileName, ios::binary | ios::in );
  ostringstream ob;
  int           offset = 0;
  char          c;


  if ( f . fail () ) return false; // if fails -> file cannot be open (does not exist, ...)

  while ( f . get ( c ) )  // if get() fails -> error reading file or EOF
  {
    if ( offset % 16 == 0 ) // display address
     cout << hex << setw ( 8 ) << setfill ( '0' ) << offset;
  
    // display the hexadecimal byte value. Note the conversion (unsigned char -> then int).
    cout << ' ' << hex << setw ( 2 ) << (int)(unsigned char)c;
    if ( ! isprint ( c ) )
      ob << '?';  // if not printable -> put ? in the character representation
    else
      ob << c;


    if ( offset % 16 == 15 )
    {
      cout << ' ' << ob . str  () << endl; // assemble & output the line every 16 bytes
      ob . str ( "" );
    }
    offset ++;
  }

  if ( offset % 16 != 0 ) // display any remaining data we have buffered
    cout << setfill ( ' ' ) << setw ( ( 16 - offset % 16 ) * 3 + 1 ) << "" << ob . str () << endl;

  if ( ! f . eof () )
  {
    f . close ();    // left the loop and ! eof -> error reading the file.
    return false;
  }
  f . close (); // close files. Always. As soon as you do not need them.
  return true;
}


int main ( int argc, char * argv [] )
{
  if ( argc != 2 )
  {
    cout << "Usage: dump <filename>" << endl;
    return 1;
  }

  if ( ! dumpFile ( argv[1] ) )
  {
    cout << "File read error." << endl;
    return 1;
  }
  return 0;
}

