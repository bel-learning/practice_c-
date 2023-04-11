#include <iostream>
#include <iomanip>
#include <cctype>
#include "expr.h"
#include "tree.h"
using namespace std;


//=================================================================================================
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
//=================================================================================================
ANode              CParser::Parse                          ( const char * str )
{
  CParser  a ( str );
  return  a . parse ();
}
//-------------------------------------------------------------------------------------------------
                   CParser::CParser                        ( const char * str )
                   : m_Str ( str ),
                     m_Pos ( str ),
                     m_PrevPos ( str ),
                     m_Token ( EToken::LEX_EOF )
{
}
//-------------------------------------------------------------------------------------------------
void               CParser::throwException                 ( const char * errDesc )
{
  throw CParseException ( errDesc, m_Str, m_PrevPos - m_Str );
}
//-------------------------------------------------------------------------------------------------
ANode              CParser::parse                          ( void )
{
  m_Token = nextToken ();

  ANode res = n_e ();
  if ( m_Token != EToken::LEX_EOF )
    throwException ( "Neocekavane znaky za koncem vyrazu" );
  return res;
}
//-------------------------------------------------------------------------------------------------
CParser::EToken    CParser::nextToken            ( void )
{
  m_Int = 0;
  while ( isspace ( *m_Pos ) ) 
    m_Pos ++;
  m_PrevPos = m_Pos;
  if ( *m_Pos == 0 ) 
    return EToken::LEX_EOF;

  switch ( * m_Pos )
  {
    case '+':
      m_Pos ++;
      return EToken::LEX_ADD;
    
    case '-':
      m_Pos ++;
      return EToken::LEX_SUB;
    
    case '*':
      m_Pos ++;
      return EToken::LEX_MUL;
    
    case '/':
      m_Pos ++;
      return EToken::LEX_DIV;
    
    case '(':
      m_Pos ++;
      return EToken::LEX_LPA;
    
    case ')':
      m_Pos ++;
      return EToken::LEX_RPA;
    
    case '[':
      m_Pos ++;
      return EToken::LEX_LSQ;
    
    case ']':
      m_Pos ++;
      return EToken::LEX_RSQ;
  }

  if ( ! memcmp ( m_Pos, "sqrt", 4 ) )
  {
    m_Pos += 4;
    return EToken::LEX_SQRT;
  }

  if ( ! isdigit ( *m_Pos ) ) 
    return EToken::LEX_ERR;

  m_Int = *m_Pos ++ - '0';

  while ( isdigit ( * m_Pos ) )
    m_Int = 10 * m_Int + *m_Pos ++ - '0';

  return EToken::LEX_INT;
}
//-------------------------------------------------------------------------------------------------
ANode              CParser::n_e                 ( void )
{
  ANode res = n_t ();
  
  while ( true )
    switch ( m_Token )
    {
      case EToken::LEX_ADD:
        m_Token = nextToken ();
        res = make_shared<CAddNode> ( res, n_t () );
        break;
      
      case EToken::LEX_SUB:
        m_Token = nextToken ();
        res = make_shared<CSubNode> ( res, n_t () );
        break;
      
      default:
        return res;
    }
}
//-------------------------------------------------------------------------------------------------
ANode              CParser::n_t                 ( void )
{
  ANode res = n_f ();
  
  while ( true )
    switch ( m_Token )
    {
      case EToken::LEX_MUL:
        m_Token = nextToken ();
        res = make_shared<CMulNode> ( res, n_f () );
        break;
      
      case EToken::LEX_DIV:
        m_Token = nextToken ();
        res = make_shared<CDivNode> ( res, n_f () );
        break;
      
      default:
        return res;
    }
}
//-------------------------------------------------------------------------------------------------
ANode              CParser::n_f                 ( void )
{
  ANode res;

  switch ( m_Token )
  {
    case EToken::LEX_INT:
      res = make_shared<CNumNode> ( m_Int );
      m_Token = nextToken ();
      return res;

    case EToken::LEX_LPA:
      m_Token = nextToken ();
      res = n_e ();
      if ( m_Token != EToken::LEX_RPA )
        throwException ( "Chybi zaviraci zavorka" );
      m_Token = nextToken ();
      return make_shared<CParNode> ( res );

    case EToken::LEX_LSQ:
      m_Token = nextToken ();
      res = n_e ();
      if ( m_Token != EToken::LEX_RSQ )
        throwException ( "Chybi zaviraci zavorka" );
      m_Token = nextToken ();
      return make_shared<CSqBrNode> ( res );

    case EToken::LEX_SQRT:
      m_Token = nextToken ();
      if ( m_Token != EToken::LEX_LPA )
        throwException ( "Ocekavam zavorku za sqrt" );
      m_Token = nextToken ();
      res = n_e ();

      if ( m_Token != EToken::LEX_RPA )
        throwException ( "Chybi zaviraci zavorka" );

      m_Token = nextToken ();
      return make_shared<CSqrtNode> ( res );

    default:
      throwException ( "Ocekavam cislo, zavorku nebo sqrt" );
      return res;
  }
}
//-------------------------------------------------------------------------------------------------
