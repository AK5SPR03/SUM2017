/* FILE NAME: T03POLE.C
 * PROGRAMMER: AK5
 * DATE: 04.06.17
 * PURPOSE: Drawing magnetic arrows
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Pole.h"

#pragma warning(disable: 4244)

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
  wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
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

  hWnd = CreateWindow(WND_CLASS_NAME, "POLE", WS_OVERLAPPEDWINDOW, 
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
  INT i, j;
  HDC hDC;
  POINT pt;
  PAINTSTRUCT ps;
  static INT w, h, t;
  static INT rndConst = 0;
  static HDC hMemDC;
  static HBITMAP hBm = NULL;
  static BOOL mode = 0;
  
  t = clock();

  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 23, 0);
    break;
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 23, 30, NULL);
    break;
  case WM_TIMER:
    srand(time(NULL));
    if (!rndConst)
      rndConst = rand() % 666 + 1000;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    srand(rndConst);
    SelectObject(hMemDC, GetStockObject(BLACK_BRUSH));
    Rectangle(hMemDC, 1, 1, w - 1, h - 1);
    if (!mode)
    for (i = w % 75; i <= w; i += 73)
      for (j = h % 75; j <= h; j += 73)
        DrawArrow(hMemDC, i, j, pt.x, pt.y);
    else 
      for (i = 0; i < 173; i++)
         DrawArrow(hMemDC, rand() % w + 13 * sin(3 * t), rand() % (h + 1) - 17 * cos(2 * t), pt.x, pt.y);
    InvalidateRect(hWnd, NULL, FALSE);
    UpdateWindow(hWnd);
    break;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    break;
  case WM_KEYDOWN:
    switch (wParam)
    {
    case VK_ESCAPE:
      DestroyWindow(hWnd);
      break;
    case '1':
      mode = 0;
      break;
    case '2':
      mode = 1;
      break;
    }
    break;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    PostQuitMessage(0);
    KillTimer(hWnd, 23);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of MyWindowFunc */

/* EOF 'T03POLE.C' */