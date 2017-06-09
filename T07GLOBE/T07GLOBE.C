/* FILE NAME: T07GLOBE.C
 * PROGRAMMER: AK5
 * DATE: 07.06.17
 * PURPOSE: Drawing sphere
 */

#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include "Sphere.h"

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

  hWnd = CreateWindow(WND_CLASS_NAME, "Globe", WS_OVERLAPPEDWINDOW, 
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
  const INT sr = 7;
  const INT mr = 80;
  static INT w, h;
  static INT rndConst = 0;
  static FLOAT secR, sec1, minR, hR;
  static HDC hMemDC;
  static HBITMAP hBm = NULL;
  
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
    SphereDraw(hMemDC, w, h);
    SendMessage(hWnd, WM_TIMER, 23, 0);
    break;
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 23, 100, NULL);

    SetBkMode(hMemDC, RGB(200, 200, 200));
    SelectObject(hMemDC, GetStockObject(BLACK_BRUSH));
    Rectangle(hMemDC, 0, 0, w - 1, h - 1);
    SphereBuild(200);

    break;
  case WM_TIMER:
    SelectObject(hMemDC, GetStockObject(BLACK_BRUSH));
    Rectangle(hMemDC, 0, 0, w - 1, h - 1);
    SphereBuild(4);
    SphereDraw(hMemDC, w, h);
    InvalidateRect(hWnd, NULL, FALSE);
    break;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    break;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
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

/* EOF 'T07GLOBE.C' */