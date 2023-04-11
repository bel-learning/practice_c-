#include <iostream>
#include <iomanip>
#include <cctype>
using namespace std;
#include "expr.h"
#include "tree.h"


//-------------------------------------------------------------------------------------------------
                   CParseException::CParseException        ( const char * errDesc,
                                                             std::string  str,
                                                             int          pos )
                   : m_ErrDesc ( errDesc ),
                     m_Str ( str ),
                     m_Pos ( pos )
{
}
//-------------------------------------------------------------------------------------------------
void               CParseException::Print                  ( void ) const
{
  cout << m_ErrDesc << '\n'
       << m_Str << '\n'
       << setw ( m_Pos ) << "" << '^' << endl;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
CNode            * CParser::Parse                          ( const char * str )
{
  CParser  a ( str );
  return  a . parse ();
}
//-------------------------------------------------------------------------------------------------
                   CParser::CParser                        ( const char * str )
                   : m_Str ( str ),
                     m_Pos ( str ),
                     m_PrevPos ( str )
{
}
//-------------------------------------------------------------------------------------------------
void               CParser::throwException                 ( const char * errDesc )
{
  throw CParseException ( errDesc, m_Str, m_PrevPos - m_Str );
}
//-------------------------------------------------------------------------------------------------
CNode            * CParser::parse                          ( void )
{
  m_Token = nextToken ();

  CNode * res = n_e ();
  if ( m_Token != LEX_EOF )
  {
    delete res;
    throwException ( "Neocekavane znaky za koncem vyrazu" );
  }
  return res;
}
//-------------------------------------------------------------------------------------------------
int                CParser::nextToken            ( void )
{
  m_Int = 0;
  while ( isspace ( *m_Pos ) ) 
    m_Pos ++;
  m_PrevPos = m_Pos;
  if ( *m_Pos == 0 ) 
    return LEX_EOF;

  switch ( * m_Pos )
  {
    case '+':
      m_Pos ++;
      return LEX_ADD;
    
    case '-':
      m_Pos ++;
      return LEX_SUB;
    
    case '*':
      m_Pos ++;
      return LEX_MUL;
    
    case '/':
      m_Pos ++;
      return LEX_DIV;
    
    case '(':
      m_Pos ++;
      return LEX_LPA;
    
    case ')':
      m_Pos ++;
      return LEX_RPA;
    
    case '[':
      m_Pos ++;
      return LEX_LSQ;
    
    case ']':
      m_Pos ++;
      return LEX_RSQ;
  }

  if ( ! memcmp ( m_Pos, "sqrt", 4 ) )
  {
    m_Pos += 4;
    return LEX_SQRT;
  }

  if ( ! isdigit ( *m_Pos ) ) 
    return LEX_ERR;

  m_Int = *m_Pos ++ - '0';

  while ( isdigit ( * m_Pos ) )
    m_Int = 10 * m_Int + *m_Pos ++ - '0';

  return LEX_INT;
}
//-------------------------------------------------------------------------------------------------
CNode            * CParser::n_e                 ( void )
{
  CNode * op = nullptr;

  try
  {
    op = n_t ();
    while ( 1 )
      switch ( m_Token )
      {
        case LEX_ADD:
          m_Token = nextToken ();
          op = new CAddNode ( op, n_t () );
          break;
        
        case LEX_SUB:
          m_Token = nextToken ();
          op = new CSubNode ( op, n_t () );
          break;
        
        default:
          return op;
      }
  }
  catch ( const CParseException & e )
  {
    delete op;
    throw;
  }
  return nullptr;
}
//-------------------------------------------------------------------------------------------------
CNode            * CParser::n_t                 ( void )
{
  CNode * op = nullptr;

  try
  {
    op = n_f ();
    while ( 1 )
      switch ( m_Token )
      {
        case LEX_MUL:
          m_Token = nextToken ();
          op = new CMulNode ( op, n_f () );
          break;
        
        case LEX_DIV:
          m_Token = nextToken ();
          op = new CDivNode ( op, n_f () );
          break;
        
        default:
          return op;
      }
  }
  catch ( const CParseException & e )
  {
    delete op;
    throw;
  }
  return nullptr;
}
//-------------------------------------------------------------------------------------------------
CNode            * CParser::n_f                 ( void )
{
  CNode * res;

  switch ( m_Token )
  {
    case LEX_INT:
      res = new CNumNode ( m_Int );
      m_Token = nextToken ();
      return res;

    case LEX_LPA:
      m_Token = nextToken ();
      res = n_e ();
      if ( m_Token != LEX_RPA )
      {
        delete res;
        throwException ( "Chybi zaviraci zavorka" );
      }

      m_Token = nextToken ();
      return new CParNode ( res );

    case LEX_LSQ:
      m_Token = nextToken ();
      res = n_e ();
      if ( m_Token != LEX_RSQ )
      {
        delete res;
        throwException ( "Chybi zaviraci zavorka" );
      }

      m_Token = nextToken ();
      return new CSqBrNode ( res );

    case LEX_SQRT:
      m_Token = nextToken ();

      if ( m_Token != LEX_LPA )
        throwException ( "Ocekavam zavorku za sqrt" );
      m_Token = nextToken ();

      res = n_e ();

      if ( m_Token != LEX_RPA )
      {
        delete res;
        throwException ( "Chybi zaviraci zavorka" );
      }

      m_Token = nextToken ();
      return new CSqrtNode ( res );

    default:
      throwException ( "Ocekavam cislo, zavorku nebo sqrt" );
      return nullptr;
  }
}
//-------------------------------------------------------------------------------------------------
