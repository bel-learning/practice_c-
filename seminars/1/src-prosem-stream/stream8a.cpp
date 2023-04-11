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
 *
 * Improvement: scanInput function is split into 3 functions, the resulting functions 
 * are much shorter. On the other hand, the code and the corresponding DFA state is 
 * not that obvious.
 */

const int          TYPE_INT                                = 0;
const int          TYPE_FLOAT                              = 1;
const int          TYPE_ERROR                              = 2;
const int          TYPE_EOF                                = -1;
//-------------------------------------------------------------------------------------------------
int                processExponent                         ( istream         & is,
                                                             double          & value )
{
  is . get (); // 'e' character
  
  int expSign = 1;
  int expVal = 0;
  
  // state 5 in DFA
  if ( is . peek () == '+' )
    is . get ();
  else if ( is . peek () == '-' )
  {
    expSign = -1;
    is . get ();
  }
  // states 5/6 in DFA
  if ( ! isdigit ( is . peek () ) )
    return TYPE_ERROR;
  // state 7
  while ( isdigit ( is . peek () ) )
    expVal = 10 * expVal + is . get () - '0';
  value *= pow ( 10, expSign * expVal );
  return TYPE_FLOAT;
}
//-------------------------------------------------------------------------------------------------
int                processFraction                         ( istream         & is,
                                                             int               valueSign,
                                                             double          & value )
{
  is . get (); // decimal point
  
  double fracVal = 0;
  int fracPow = 0;
  
  // state 3
  if ( ! isdigit ( is . peek () ) )
    return TYPE_ERROR;
  // state 4
  while ( isdigit ( is . peek () ) )
  {  
    fracVal = 10 * fracVal + is . get () - '0';
    fracPow ++;
  }
  value = valueSign * ( value + fracVal / pow ( 10, fracPow ) );
  if ( tolower ( is . peek () ) == 'e' )
    return processExponent ( is, value );
  return TYPE_FLOAT;
}
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
  // state 0
  while ( isspace ( is . peek () ) )
    is . get ();
  
  if ( is . peek () == EOF )
    return TYPE_EOF;

  floatVal = intVal = 0;
  int valSign = 1;
  
  if ( is . peek () == '+' )
    is . get ();
  else if ( is . peek () == '-' )
  {
    valSign = -1;
    is . get ();
  }
  // state 0/1
  if ( is . peek () == '.' )
    return processFraction ( is, valSign, floatVal );
  
  if ( ! isdigit ( is . peek () ) )
    return TYPE_ERROR;
  
  // state 2
  while ( isdigit ( is . peek () ) )
  {
    int d = is . get () - '0';
    intVal = 10 * intVal + d;
    floatVal = 10 * floatVal + d;
  }
  
  if ( tolower ( is . peek () ) == 'e' )
  {
    floatVal *= valSign;
    return processExponent ( is, floatVal );
  }
  
  if ( is . peek () == '.' )
    return processFraction ( is, valSign, floatVal );

  intVal *= valSign;
  return TYPE_INT;
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
