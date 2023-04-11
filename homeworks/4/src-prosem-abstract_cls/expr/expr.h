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
    static CNode           * Parse               ( const char * Str );

  protected:

    const char             * m_Str;
    const char             * m_Pos;
    const char             * m_PrevPos;
    int                      m_Int;
    int                      m_Token;

    static const int         LEX_EOF        = 0;
    static const int         LEX_ERR        = 1;
    static const int         LEX_INT        = 2;
    static const int         LEX_ADD        = 3;
    static const int         LEX_SUB        = 4;
    static const int         LEX_MUL        = 5;
    static const int         LEX_DIV        = 6;
    static const int         LEX_LPA        = 7;
    static const int         LEX_RPA        = 8;
    static const int         LEX_LSQ        = 9;
    static const int         LEX_RSQ        = 10;
    static const int         LEX_SQRT       = 11;

                             CParser             ( const char * Str );
    CNode                  * parse               ( void );

    int                      nextToken           ( void );
    void                     throwException      ( const char * );

    CNode                  * n_e                 ( void );
    CNode                  * n_t                 ( void );
    CNode                  * n_f                 ( void );
};

#endif /* PA2_EXPR_H */
