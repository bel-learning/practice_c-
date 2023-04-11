#include <iostream>
#include <iomanip>
#include <windows.h>
#include "tree.h"
#include "expr.h"
using namespace std;

static const char mainWndClass [] = "MainWndClass";
static ANode g_Tree;
static int   g_FontSize = 10;


//-------------------------------------------------------------------------------------------------
static LRESULT CALLBACK mainWndCallback              ( HWND hwnd, unsigned Msg, WPARAM wparam, LPARAM lparam )
{
  PAINTSTRUCT ps;
  HFONT oldFont, newFont;

  switch ( Msg )
  {
    case WM_DESTROY:
      PostQuitMessage ( 0 );
      break;
    
    case WM_CHAR:
      if ( wparam == '+' && g_FontSize < 100 )
      {
        g_FontSize ++;
        g_Tree -> ResetSize ();
        InvalidateRect ( hwnd, nullptr, TRUE );
        UpdateWindow ( hwnd );
      }
      if ( wparam == '-' && g_FontSize > 10 )
      {
        g_FontSize --;
        g_Tree -> ResetSize ();
        InvalidateRect ( hwnd, nullptr, TRUE );
        UpdateWindow ( hwnd );
      }
      break;
      
    
    case WM_PAINT:
      BeginPaint ( hwnd, &ps );
      newFont = CreateFont ( g_FontSize, 0, 0, 0, FW_NORMAL, 
                             0, 0, 0, 
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_MODERN,
                             "Courier New" );
      oldFont = (HFONT) SelectObject ( ps . hdc, newFont );
      g_Tree -> GetSize ( ps . hdc );
      g_Tree -> Draw ( ps . hdc, 20, 20 );
      DeleteObject ( SelectObject ( ps . hdc, oldFont ) );
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
  return 0;
}
//-------------------------------------------------------------------------------------------------
