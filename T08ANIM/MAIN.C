/* FILE NAME: MAIN.C
 * PROGRAMMER: AK5
 * DATE: 09.06.17
 * PURPOSE: Main
 */

#include "Anim.h" 
#include "Units.h"

#define WND_CLASS_NAME "My window class"


/*defenition*/
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* Main prog func */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR* CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetStockObject(WHITE_BRUSH);
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "ERR register", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "Anim", WS_OVERLAPPEDWINDOW, 
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, 
      hInstance, NULL);

  ShowWindow(hWnd, SW_NORMAL);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
} /* End of WinMain */

/* MyWindowFunc */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *MinMax;
  static INT w, h;
  static HDC hMemDC;
  static HBITMAP hBm = NULL;
  
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)lParam;
    MinMax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    AK5_AnimResize(LOWORD(lParam), HIWORD(lParam));
    AK5_AnimRender();
    break;
  case WM_CREATE:
    AK5_AnimInit(hWnd);
    SetTimer(hWnd, 239, 30, NULL);
    break;
  case WM_TIMER:
    AK5_AnimRender();    
    InvalidateRect(hWnd, NULL, FALSE);
    break;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    AK5_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    break;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
    if (wParam == 'F')
      AK5_AnimFlipFullScreen();
    break;
  case WM_MOUSEWHEEL:
    AK5_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_DESTROY:
    AK5_AnimClose();
    PostQuitMessage(239);
    KillTimer(hWnd, 239);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of MyWindowFunc */


/* EOF 'MAIN.C' */