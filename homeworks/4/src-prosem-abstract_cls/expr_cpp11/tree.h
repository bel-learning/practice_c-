#ifndef PA2_TREE_H
#define PA2_TREE_H

#include <windows.h>
#include <memory>


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
    virtual                 ~CNode               ( void ) = default;
    virtual void             Draw                ( HDC hdc, int x, int y ) = 0;
    virtual void             ResetSize           ( void );
    CSize                    GetSize             ( HDC hdc );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const = 0;
    CSize                    m_Size;
    bool                     m_Initialized;
};

typedef std::shared_ptr<CNode>                   ANode;

class CParNode : public CNode
{
  public:
                             CParNode            ( ANode child );
    virtual void             Draw                ( HDC hdc, int x, int y );
    virtual void             ResetSize           ( void );
  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
    ANode                    m_Child;
};

class CSqBrNode : public CNode
{
  public:
                             CSqBrNode           ( ANode child );
    virtual void             Draw                ( HDC hdc, int x, int y );
    virtual void             ResetSize           ( void );
  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
    ANode                    m_Child;
};


class CSqrtNode : public CNode
{
  public:
                             CSqrtNode           ( ANode child );
    virtual void             Draw                ( HDC hdc, int x, int y );
    virtual void             ResetSize           ( void );
  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
    ANode                    m_Child;
};

class CBHorNode : public CNode
{
  public:
                             CBHorNode           ( ANode l, ANode r );
    virtual void             Draw                ( HDC hdc, int x, int y );
    virtual void             ResetSize           ( void );
  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
    virtual const char     * getOperator         ( void ) const = 0;
    ANode                    m_L, m_R;
};

class CNumNode : public CNode
{
  public:
                             CNumNode            ( int val );
    virtual void             Draw                ( HDC hdc, int x, int y );

  protected:
    virtual CSize            getSize             ( HDC hdc ) const;

    int                      m_Val;
};

class CAddNode : public CBHorNode
{
  public:
                             CAddNode            ( ANode l, ANode r );


  protected:
    virtual const char     * getOperator         ( void ) const;
};

class CSubNode : public CBHorNode
{
  public:
                             CSubNode            ( ANode l, ANode r );


  protected:
    virtual const char     * getOperator         ( void ) const;
};

class CMulNode : public CBHorNode
{
  public:
                             CMulNode            ( ANode l, ANode r );


  protected:
    virtual const char     * getOperator         ( void ) const;
};


class CDivNode : public CNode
{
  public:
                             CDivNode            ( ANode l, ANode r );
    virtual void             Draw                ( HDC hdc, int x, int y );
    virtual void             ResetSize           ( void );
  protected:
    virtual CSize            getSize             ( HDC hdc ) const;
    ANode                    m_L, m_R;
};


#endif /* PA2_TREE_H */
