#include <cstdio>
#include <algorithm>
#include "tree.h"
using namespace std;

//=================================================================================================
                   CNode::CNode                  ( void )
                   : m_Size ( 0, 0, 0 ),
                     m_Initialized ( false )
{
}
//-------------------------------------------------------------------------------------------------
CSize              CNode::GetSize                ( HDC hdc )
{
  if ( ! m_Initialized )
  {
    m_Size        = getSize ( hdc );
    m_Initialized = true;
  }
  return m_Size;
}
//-------------------------------------------------------------------------------------------------
void               CNode::ResetSize              ( void )
{
  m_Initialized = false;
}
//=================================================================================================
                   CNumNode::CNumNode            ( int val )
                   : m_Val ( val )
{
}
//-------------------------------------------------------------------------------------------------
void               CNumNode::Draw                ( HDC hdc, int x, int y )
{
  char txt[30];

  snprintf ( txt, sizeof ( txt ), "%d", m_Val );
  TextOut  ( hdc,  x, y, txt, strlen ( txt ) );
}
//-------------------------------------------------------------------------------------------------
CSize              CNumNode::getSize             ( HDC hdc ) const
{
  char  txt[30];
  SIZE  sz;

  snprintf             ( txt, sizeof ( txt ), "%d", m_Val );
  GetTextExtentPoint32 ( hdc, txt, strlen ( txt ), &sz );
  return CSize ( sz . cx, sz . cy, sz . cy / 2 );
}
//=================================================================================================
                   CSqBrNode::CSqBrNode          ( ANode child ) 
                   : m_Child ( child )
{
}
//-------------------------------------------------------------------------------------------------
void               CSqBrNode::Draw               ( HDC hdc, int x, int y )
{
  m_Child -> Draw ( hdc, x + 5, y + 2 );

  MoveToEx ( hdc, x + 3, y, nullptr );
  LineTo   ( hdc, x,     y );
  LineTo   ( hdc, x,     y + m_Size . m_H - 1 );
  LineTo   ( hdc, x + 3, y + m_Size . m_H - 1 );

  MoveToEx ( hdc, x + m_Size . m_W - 1 - 3, y, nullptr );
  LineTo   ( hdc, x + m_Size . m_W - 1, y );
  LineTo   ( hdc, x + m_Size . m_W - 1, y + m_Size . m_H - 1 );
  LineTo   ( hdc, x + m_Size . m_W - 1 - 3, y + m_Size . m_H - 1 );
}
//-------------------------------------------------------------------------------------------------
CSize              CSqBrNode::getSize            ( HDC hdc ) const
{
  CSize sz = m_Child -> GetSize ( hdc );
  return CSize ( sz . m_W + 10, sz . m_H + 4, sz . m_Bl + 2 );
}
//-------------------------------------------------------------------------------------------------
void               CSqBrNode::ResetSize          ( void )
{
  m_Child -> ResetSize ();
  m_Initialized = false;
}
//=================================================================================================
                   CParNode::CParNode            ( ANode child ) 
                   : m_Child ( child )
{
}
//-------------------------------------------------------------------------------------------------
void               CParNode::Draw                ( HDC hdc, int x, int y )
{
  m_Child -> Draw ( hdc, x + 5, y + 2 );

  Arc ( hdc, x, y, x + 7, y + m_Size . m_H - 1, x + 3, y, x + 3, y + m_Size . m_H - 1 );
  Arc ( hdc, x + m_Size . m_W - 1 - 7,  y, x + m_Size . m_W - 1, y + m_Size . m_H - 1,
         x + m_Size . m_W - 7 - 1 + 4, y + m_Size . m_H - 1, x + m_Size . m_W - 7 - 1 + 4, 0 );

}
//-------------------------------------------------------------------------------------------------
CSize              CParNode::getSize             ( HDC hdc ) const
{
  CSize sz = m_Child -> GetSize ( hdc );
  return CSize ( sz . m_W + 10, sz . m_H + 4, sz . m_Bl + 2 );
}
//-------------------------------------------------------------------------------------------------
void               CParNode::ResetSize           ( void )
{
  m_Child -> ResetSize ();
  m_Initialized = false;
}
//=================================================================================================
                   CSqrtNode::CSqrtNode          ( ANode child ) 
                   : m_Child ( child )
{
}
//-------------------------------------------------------------------------------------------------
void               CSqrtNode::Draw               ( HDC hdc, int x, int y )
{
  m_Child -> Draw ( hdc, x + 10, y + 4 );

  MoveToEx ( hdc, x, y + 2 * m_Size . m_H / 3, nullptr );
  LineTo   ( hdc, x + 5, y + m_Size . m_H - 1 );
  LineTo   ( hdc, x + 10, y );
  LineTo   ( hdc, x + m_Size . m_W - 1 , y );
}
//-------------------------------------------------------------------------------------------------
CSize              CSqrtNode::getSize            ( HDC hdc ) const
{
  CSize sz = m_Child -> GetSize ( hdc );
  return CSize ( sz . m_W + 10, sz . m_H + 4, sz . m_Bl + 4 );
}
//-------------------------------------------------------------------------------------------------
void               CSqrtNode::ResetSize          ( void )
{
  m_Child -> ResetSize ();
  m_Initialized = false;
}
//=================================================================================================
                   CBHorNode::CBHorNode          ( ANode l, ANode r ) 
                   : m_L ( l ),
                     m_R ( r )
{
}
//-------------------------------------------------------------------------------------------------
CSize              CBHorNode::getSize            ( HDC hdc ) const
{
  CSize        lsz = m_L -> GetSize ( hdc );
  CSize        rsz = m_R -> GetSize ( hdc );
  const char * op = getOperator ();
  SIZE         sz;
  
  GetTextExtentPoint32 ( hdc, op, strlen ( op ), &sz );
  return CSize ( lsz . m_W + 5 + sz . cx + 5 + rsz . m_W,
                 max ( lsz . m_Bl, rsz . m_Bl ) + max ( lsz . m_H - lsz . m_Bl, rsz . m_H - rsz . m_Bl ),
                 max ( lsz . m_Bl, rsz . m_Bl ) );
}
//-------------------------------------------------------------------------------------------------
void               CBHorNode::Draw               ( HDC hdc, int x, int y )
{
  CSize        lsz = m_L -> GetSize ( hdc );
  CSize        rsz = m_R -> GetSize ( hdc );
  const char * op = getOperator ();
  SIZE         sz;

  GetTextExtentPoint32 ( hdc, op, strlen ( op ), &sz );
  m_L -> Draw ( hdc, x, y + m_Size . m_Bl - lsz . m_Bl );
  TextOut ( hdc, x + lsz . m_W + 5, y + m_Size . m_Bl - sz . cy / 2, op, strlen ( op ) );
  m_R -> Draw ( hdc, x + lsz . m_W + 5 + sz . cx + 5, y + m_Size . m_Bl - rsz . m_Bl );
}
//-------------------------------------------------------------------------------------------------
void               CBHorNode::ResetSize          ( void )
{
  m_L -> ResetSize ();
  m_R -> ResetSize ();
  m_Initialized = false;
}
//=================================================================================================
                   CAddNode::CAddNode            ( ANode l, ANode r ) 
                   : CBHorNode ( l, r )
{
}
//-------------------------------------------------------------------------------------------------
const char       * CAddNode::getOperator         ( void ) const
{
  return "+";
}
//=================================================================================================
                   CSubNode::CSubNode            ( ANode l, ANode r ) 
                   : CBHorNode ( l, r )
{
}
//-------------------------------------------------------------------------------------------------
const char       * CSubNode::getOperator         ( void ) const
{
  return "-";
}
//=================================================================================================
                   CMulNode::CMulNode            ( ANode l, ANode r ) 
                   : CBHorNode ( l, r )
{
}
//-------------------------------------------------------------------------------------------------
const char       * CMulNode::getOperator         ( void ) const
{
  return "*";
}
//=================================================================================================
                   CDivNode::CDivNode            ( ANode l, ANode r ) 
                   : m_L ( l ),
                     m_R ( r )
{
}
//-------------------------------------------------------------------------------------------------
CSize              CDivNode::getSize             ( HDC hdc ) const
{
  CSize        lsz = m_L -> GetSize ( hdc );
  CSize        rsz = m_R -> GetSize ( hdc );

  return CSize ( max ( lsz . m_W, rsz . m_W ) + 6, 
                 lsz . m_H + 5 + rsz . m_H,
                 lsz . m_H + 3 );
}
//-------------------------------------------------------------------------------------------------
void               CDivNode::Draw                ( HDC hdc, int x, int y )
{
  CSize        lsz = m_L -> GetSize ( hdc );
  CSize        rsz = m_R -> GetSize ( hdc );

  m_L -> Draw ( hdc, x + ( m_Size . m_W - lsz . m_W ) / 2, y );
  MoveToEx ( hdc, x,                y + m_Size . m_Bl, nullptr );
  LineTo   ( hdc, x + m_Size . m_W, y + m_Size . m_Bl );
  m_R -> Draw ( hdc, x + ( m_Size . m_W - rsz . m_W ) / 2, y + m_Size . m_Bl + 5 );
}
//-------------------------------------------------------------------------------------------------
void               CDivNode::ResetSize           ( void )
{
  m_L -> ResetSize ();
  m_R -> ResetSize ();
  m_Initialized = false;
}
//-------------------------------------------------------------------------------------------------
