#ifndef PA2_TREE_H
#define PA2_TREE_H

#include <windows.h>


struct CSize
{
  public:
                             CSize               ( int w, int h, int bl )
                             : m_W ( w ),
                               m_H ( h ),
                               m_Bl ( bl )
    {
    }
    int                      m_W;
    int                      m_H;
    int                      m_Bl;
};


class CNode
{
  public:
                             CNode               ( void );
    virtual                 ~CNode               ( void ) 
    { 
    }
    virtual void             Draw                ( HDC hdc, int x, int y ) = 0;
    CSize                    GetSize             ( HDC hdc );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const = 0;
    CSize                    m_Size;
    bool                     m_Initialized;
};

class CUNode : public CNode
{
  public:
                             CUNode              ( CNode * child );
    virtual                 ~CUNode              ( void );
  protected:
    CNode                  * m_Child;
};

class CParNode : public CUNode
{
  public:
                             CParNode            ( CNode * child );
    virtual void             Draw                ( HDC hdc, int x, int y );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
};

class CSqBrNode : public CUNode
{
  public:
                             CSqBrNode           ( CNode * child );
    virtual void             Draw                ( HDC hdc, int x, int y );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
};


class CSqrtNode : public CUNode
{
  public:
                             CSqrtNode           ( CNode * child );
    virtual void             Draw                ( HDC hdc, int x, int y );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
};


class CBNode : public CNode
{
  public:
                             CBNode              ( CNode * l, CNode * r );
    virtual                 ~CBNode              ( void );

  protected:
    CNode                  * m_L, * m_R;
};

class CBHorNode : public CBNode
{
  public:
                             CBHorNode           ( CNode * l, CNode * r );
    virtual void             Draw                ( HDC hdc, int x, int y );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
    virtual const char     * getOperator         ( void ) const = 0;
};

class CNumNode : public CNode
{
  public:
                             CNumNode            ( int X );
    virtual void             Draw                ( HDC hdc, int x, int y );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const;

    int                      m_Val;
};

class CAddNode : public CBHorNode
{
  public:
                             CAddNode            ( CNode * l, CNode * r );


  protected:
    virtual const char     * getOperator         ( void ) const;
};

class CSubNode : public CBHorNode
{
  public:
                             CSubNode            ( CNode * l, CNode * r );


  protected:
    virtual const char     * getOperator         ( void ) const;
};

class CMulNode : public CBHorNode
{
  public:
                             CMulNode            ( CNode * l, CNode * r );


  protected:
    virtual const char     * getOperator         ( void ) const;
};


class CDivNode : public CBNode
{
  public:
                             CDivNode            ( CNode * l, CNode * r );
    virtual void             Draw                ( HDC hdc, int x, int y );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
};


#endif /* PA2_TREE_H */
