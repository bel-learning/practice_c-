#include <iostream>
#include <iomanip>
#include <cctype>
#include <climits>
#include <cmath>
using namespace std;

/** Read input stream, try to process it as a number:
 *  - skip whitespace
 *  - try to read a base 10 integer or
 *  - try to read a floating point number (possibly with a fraction/exponent/both),
 * This implementation does not handle decimals in base 2/8/16.
 */

const int          TYPE_INT                                = 0;
const int          TYPE_FLOAT                              = 1;
const int          TYPE_ERROR                              = 2;
const int          TYPE_EOF                                = -1;
//-------------------------------------------------------------------------------------------------
/**
 * Read the next number. Skip whitespace, scan int/float. Report an error if there is not a valid 
 * number in the input.
 * 
 * @param[in] is         input stream to read the data from
 * @param[out] intVal    the value of the integer (if TYPE_INT is returned)
 * @param[out] floatVal  the value of the integer (if TYPE_FLOAT is returned)
 * @return one of TYPE_INT/TYPE_FLOAT/TYPE_ERROR/TYPE_EOF
 * @note the function is AWFUL. The implementation directly follows the structure of the DFA; the 
 *    body is very long.
 */
int                scanInput                               ( istream         & is,
                                                             int             & intVal,
                                                             double          & floatVal )
{
  int state = 0;
  int valSign = 1, expSign = 1;
  int fracPow = 0, expVal = 0;
  double fracVal = 0;
  
  while ( true )
  {
    int c = is . peek ();
    switch ( state )
    {
      case 0:
        if ( isspace ( c ) )
        {
          is . get ();
          break;
        }
        if ( c == '-' || c == '+' )
        {
          valSign = c == '+' ? 1 : -1;
          is . get ();
          state = 1;
          break;
        }
        if ( isdigit ( c ) )
        {
          floatVal = intVal = c - '0';
          is . get ();
          state = 2;
          break;
        }
        if ( c == '.' )
        {
          floatVal = intVal = 0;
          is . get  ();
          state = 3;
        }
        if ( c == EOF )
          return TYPE_EOF;
        return TYPE_ERROR;
      
      case 1:
        if ( isdigit ( c ) )
        {
          floatVal = intVal = c - '0';
          is . get ();
          state = 2;
          break;
        }
        if ( c == '.' )
        {
          is . get ();
          state = 3;
          break;
        }
        return TYPE_ERROR;
      
      case 2:
        if ( isdigit ( c ) )
        {
          intVal = intVal * 10 + c - '0';
          floatVal = floatVal * 10 + c - '0';
          is . get ();
          break;
        }
        if ( c == '.' )
        {
          is . get ();
          state = 3;
          break;
        }
        if ( tolower ( c ) == 'e' )
        {
          is . get ();
          state = 5;
          break;
        }
        intVal *= valSign;
        return TYPE_INT;
        
      case 3:
        if ( isdigit ( c ) )
        {
          fracVal = c - '0';
          fracPow ++;
          is . get ();
          state = 4;
          break;
        }
        return TYPE_ERROR;
        
      case 4:
        if ( isdigit ( c ) )
        {
          fracVal = 10 * fracVal + c - '0';
          fracPow ++;
          is . get ();
          break;
        }
        if ( tolower ( c ) == 'e' )
        {
          is . get ();
          state = 5;
          break;
        }
        floatVal = valSign * ( floatVal + fracVal / pow ( 10, fracPow ) );
        return TYPE_FLOAT;

      case 5:
        if ( c == '+' || c == '-' )
        {
          expSign = c == '+' ? 1 : -1;
          is . get ();
          state = 6;
          break;
        }
        if ( isdigit ( c ) )
        {
          expVal = c - '0';
          is . get ();
          state = 7;
          break;
        }
        return TYPE_ERROR;
        
      case 6:
        if ( isdigit ( c ) )
        {
          expVal = c - '0';
          is . get ();
          state = 7;
          break;
        }
        return TYPE_ERROR;
        
      case 7:
        if ( isdigit ( c ) )
        {
          expVal = 10 * expVal + c - '0';
          is . get ();
          break;
        }
        floatVal = valSign * ( floatVal + fracVal / pow ( 10, fracPow ) ) * pow ( 10, expSign * expVal );
        return TYPE_FLOAT;
    }      
  }
}  
//-------------------------------------------------------------------------------------------------
int                main                                    ( void )
{
  while ( true )
  {
    int  intVal;
    double floatVal;
    switch ( scanInput ( cin, intVal, floatVal ) )
    { 
      case TYPE_INT:
        cout << "INT: " << intVal << endl;
        break;
      
      case TYPE_FLOAT:
        cout << "FLOAT: " << floatVal << endl;
        break;
      
      case TYPE_EOF:
        return 0;
      
      case TYPE_ERROR:
      default:  
        cout << "ERROR" << endl;
        return 1;
    }
  }
}
