#include <iostream>
#include <iomanip>
#include <windows.h>
#include "tree.h"
#include "expr.h"
using namespace std;

static const char mainWndClass [] = "MainWndClass";
static CNode * g_Tree;

//-------------------------------------------------------------------------------------------------
static LRESULT CALLBACK mainWndCallback              ( HWND hwnd, unsigned Msg, WPARAM wparam, LPARAM lparam )
{
  PAINTSTRUCT ps;

  switch ( Msg )
  {
    case WM_DESTROY:
      PostQuitMessage ( 0 );
      break;
    
    case WM_PAINT:
      BeginPaint ( hwnd, &ps );
      g_Tree -> GetSize ( ps . hdc );
      g_Tree -> Draw ( ps . hdc, 20, 20 );
      EndPaint ( hwnd, &ps );
      break;
  
  }
  return DefWindowProc ( hwnd, Msg, wparam, lparam );
}
//-------------------------------------------------------------------------------------------------
int                main                          ( int argc, char * argv [] )
{
  MSG        msg;
  WNDCLASS   wc;


  if ( argc != 2 )
  {
    cout << "expr <expression>" << endl;
    return 1;
  }

  try
  {
    g_Tree = CParser :: Parse ( argv[1] );
  }
  catch ( const CParseException & e )
  {
    e . Print ();
    return 1;
  }

  wc . style        = CS_HREDRAW | CS_VREDRAW;
  wc . lpfnWndProc  = mainWndCallback;
  wc . cbClsExtra   = 0;
  wc . cbWndExtra   = sizeof ( DWORD );
  wc . hInstance    = GetModuleHandle ( nullptr );
  wc . hIcon        = nullptr;
  wc . hCursor      = nullptr;
  wc . hbrBackground= (HBRUSH)GetStockObject ( WHITE_BRUSH );
  wc . lpszMenuName = nullptr;
  wc . lpszClassName= mainWndClass;

  if ( !RegisterClass ( &wc ) ) 
    return 0;

  HWND mainWnd = CreateWindow ( mainWndClass, "Expr", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  CW_USEDEFAULT,
                          nullptr, nullptr, GetModuleHandle ( nullptr ), nullptr );

  ShowWindow ( mainWnd, SW_SHOW );
  while ( GetMessage ( &msg, nullptr, 0, 0 ) )
  {
    TranslateMessage ( &msg );
    DispatchMessage ( &msg );
  }
  delete g_Tree;

  return 0;
}
//-------------------------------------------------------------------------------------------------
