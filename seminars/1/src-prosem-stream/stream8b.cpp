#include <iostream>
#include <iomanip>
#include <cctype>
#include <climits>
#include <cmath>
using namespace std;

/** Read input stream, try to process it as a number:
 *  - skip whitespace
 *  - try to read a base 2/8/10/16 integer or
 *  - try to read a floating point number (possibly with a fraction/exponent/both)
 * This implementation does not handle C++ 11 extensions (123'245, hex floating points)
 */
const int          TYPE_INT                                = 0;
const int          TYPE_FLOAT                              = 1;
const int          TYPE_ERROR                              = 2;
const int          TYPE_EOF                                = -1;
//-------------------------------------------------------------------------------------------------
int                convertDigit                            ( char              digit )
{
  if ( digit >= '0' && digit <= '9' )
    return digit - '0';
  if ( digit >= 'a' && digit <= 'z' )
    return digit - 'a' + 10;
  if ( digit >= 'A' && digit <= 'Z' )
    return digit - 'A' + 10;
  return INT_MAX;
}
//-------------------------------------------------------------------------------------------------
bool               isoctal                                 ( char              digit )
{
  return digit >= '0' && digit <= '7';
}
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
    expVal = 10 * expVal + convertDigit ( is . get () );
  
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
    fracVal = 10 * fracVal + convertDigit ( is . get () );
    fracPow ++;
  }
  value = valueSign * ( value + fracVal / pow ( 10, fracPow ) );
  if ( tolower ( is . peek () ) == 'e' )
    return processExponent ( is, value );
  return TYPE_FLOAT;
}
//-------------------------------------------------------------------------------------------------
int                processIntRadix                         ( istream         & is,
                                                             int               valSign,
                                                             int               base,
                                                             int             & intVal )
{
  int v = convertDigit ( is . peek () );
  if ( v >= base )
    return TYPE_ERROR;
  intVal = v;
  is . get ();
  
  while ( ( v = convertDigit ( is . peek () ) ) < base )
  {
    intVal = base * intVal + v;
    is . get ();
  }
  intVal *= valSign;
  return TYPE_INT;
}  
//-------------------------------------------------------------------------------------------------
int                processIntType                          ( istream         & is,
                                                             int               valSign,
                                                             int             & intVal,
                                                             double          & floatVal )
{
  is . get (); // char '0'
  
  // state 8 in DFA
  intVal = 0;
  floatVal = 0;
  if ( tolower ( is . peek () ) == 'x' )
  { 
    is . get ();    
    return processIntRadix ( is, valSign, 16, intVal );
  }  
  if ( tolower ( is . peek () ) == 'b' )
  {
    is . get ();    
    return processIntRadix ( is, valSign, 2, intVal );
  }  
  if ( isoctal ( is . peek () ) )
    return processIntRadix ( is, valSign, 8, intVal );
  if ( is . peek () == '.' )
    return processFraction ( is, valSign, floatVal );
  if ( is . peek () == 'e' )
  {  
    floatVal *= valSign;
    return processExponent ( is, floatVal );
  }  
  return TYPE_INT;
}  
//-------------------------------------------------------------------------------------------------
int                processDecimal                          ( istream         & is,
                                                             int               valSign,
                                                             int             & intVal,
                                                             double          & floatVal )
{
  intVal = 0;
  floatVal = 0;
  // state 2 in DFA
  while ( isdigit ( is . peek () ) )
  {
    char c = is . get ();
    intVal = 10 * intVal + convertDigit ( c );
    floatVal = 10.0 * floatVal + convertDigit ( c );
  }
  
  if ( is . peek () == '.' )
    return processFraction ( is, valSign, floatVal );
  
  if ( tolower ( is . peek () ) == 'e' )
  { 
    floatVal *= valSign;    
    return processExponent ( is, floatVal );
  }  
  intVal *= valSign;
  return TYPE_INT;
}
//-------------------------------------------------------------------------------------------------
int                scanInput                               ( istream         & is,
                                                             int             & intVal,
                                                             double          & floatVal )
{
  // state 0
  while ( isspace ( is . peek () ) )
    is . get ();

  int valSign = 1;
  
  if ( is . peek () == EOF )
    return TYPE_EOF;
  
  if ( is . peek () == '-' )
  {
    valSign = -1;
    is . get ();
  }
  else if ( is . peek () == '+' )
    is . get ();
  
  // state 0/1
  if ( is . peek () == '0' )
    return processIntType ( is, valSign, intVal, floatVal );
  if ( isdigit ( is . peek () ) )
    return processDecimal ( is, valSign, intVal, floatVal );
  else
    return TYPE_ERROR;
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
