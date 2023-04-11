#ifndef PA2_EXPR_H
#define PA2_EXPR_H

#include "tree.h"
#include <string>

class CParseException
{
  public:
                             CParseException     ( const char * errDesc,
                                                   std::string  str,
                                                   int          pos );

    void                     Print               ( void ) const;
  protected:
    std::string              m_ErrDesc;
    std::string              m_Str;
    int                      m_Pos;
};

class CParser
{
  public:
    static ANode             Parse               ( const char * str );

  protected:
    enum class EToken
    {
      LEX_EOF, 
      LEX_ERR, 
      LEX_INT, 
      LEX_ADD, 
      LEX_SUB, 
      LEX_MUL, 
      LEX_DIV, 
      LEX_LPA, 
      LEX_RPA, 
      LEX_LSQ, 
      LEX_RSQ, 
      LEX_SQRT
    };

    const char             * m_Str;
    const char             * m_Pos;
    const char             * m_PrevPos;
    int                      m_Int;
    EToken                   m_Token;

                             CParser             ( const char * str );
    ANode                    parse               ( void );
    EToken                   nextToken           ( void );
    void                     throwException      ( const char * );

    ANode                    n_e                 ( void );
    ANode                    n_t                 ( void );
    ANode                    n_f                 ( void );
};

#endif /* PA2_EXPR_H */
