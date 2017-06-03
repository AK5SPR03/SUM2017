/* FILE NAME: T01EYES.C
 * PROGRAMMER: AK5
 * DATE: 01.06.17
 * PURPOSE: Drawing eyes program
 */

#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>

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

  hWnd = CreateWindow(WND_CLASS_NAME, "Eyes", WS_OVERLAPPEDWINDOW, 
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

/* DrawEye */
VOID DrawEye( HDC hDC, INT xc, INT yc, INT r, INT x, INT y)
{
  INT xz, yz;
  FLOAT lx, ly;
  HBRUSH hbr1, hbr2;

  lx = (x - xc) * r / 2 / sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc));
  xz = xc + (INT)lx;
  ly = (y - yc) * r / 2 / sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc));
  yz = yc + (INT)ly;

  hbr1 = CreateSolidBrush(RGB(200, 255, 255));
  hbr2 = CreateSolidBrush(RGB(134, 69, 148));
  SelectObject(hDC, GetStockObject(BLACK_PEN));
  SelectObject(hDC, hbr1);
  Ellipse(hDC, xc - r, yc - r, xc + r, yc + r);
  SelectObject(hDC, hbr2);
  Ellipse(hDC, xz - r / 2, yz - r / 2, xz + r / 2, yz + r / 2);
  DeleteObject(hbr1);
  DeleteObject(hbr2);
} /* End of DrawEye */
/* MyWindowFunc */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i;
  HDC hDC;
  POINT pt;
  PAINTSTRUCT ps;
  static INT w, h;
  static INT rndConst = 0;
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
    for (i = 0; i < 43; i++)
      DrawEye(hMemDC, rand() % (w + 1), rand() % (h + 1), rand() % 100 + 20, pt.x, pt.y);
    InvalidateRect(hWnd, NULL, FALSE);
    UpdateWindow(hWnd);
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

/* EOF 'T01EYES.C' */