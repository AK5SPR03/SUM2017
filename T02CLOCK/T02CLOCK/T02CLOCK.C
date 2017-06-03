/* FILE NAME: T01EYES.C
 * PROGRAMMER: AK5
 * DATE: 02.06.17
 * PURPOSE: Drawing clock
 */

#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>

#pragma warning(disable: 4244)

#define WND_CLASS_NAME "My window class"
#define PI       3.14159265358979323846

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

/* MyWindowFunc */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i;
  HDC hDC;
  POINT pt;
  PAINTSTRUCT ps;
  BITMAP bm;
  const INT sr = 7;
  const INT mr = 80;
  static INT w, h;
  static INT rndConst = 0, secR, minR, hR;
  static F
  static HDC hMemDC, hMemDCLogo;
  static HBITMAP hBm = NULL, hBmAnd, hBmXor;
  static SYSTEMTIME st;
  static POINT pts[4], p[4];
  
  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    pts[0].x = w / 2;
    pts[0].y = h / 2 - mr;
    pts[1].x = w / 2 + sr;
    pts[1].y = h / 2;
    pts[2].x = w / 2;
    pts[2].y = h / 2 + sr;
    pts[3].x = w / 2 - sr;
    pts[3].y = h / 2;

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
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 23, 30, NULL);

    hBmAnd = LoadImage(NULL, "fu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXor = LoadImage(NULL, "fuXOR.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    Rectangle(hMemDC, 0, 0, w - 1, h - 1);
    SelectObject(hMemDCLogo, hBmAnd);
    GetObject(hBmAnd, sizeof(BITMAP), &bm);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight - 70) / 2, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);        /*copy - and*/
    /*SelectObject(hMemDCLogo, hBmXor);
    BitBlt(hMemDC, 100, 100, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCINVERT); */
    break;
  case WM_TIMER:
    GetLocalTime(&st);
    secR = st.wSecond / 30 * PI;


    p[0].x = w / 2 - mr * sin(secR);
    p[0].y = h / 2 + mr * cos(secR);
    p[1].x = w / 2 + sr * cos(secR);
    p[1].y = h / 2 + sr * sin(secR);
    p[2].x = w / 2 + sr * sin(secR);
    p[2].y = h / 2 - sr * cos(secR);
    p[3].x = w / 2 - sr * cos(secR);
    p[3].y = h / 2 - sr * sin(secR);

    Rectangle(hMemDC, 0, 0, w - 1, h - 1);
    SelectObject(hMemDCLogo, hBmAnd);
    GetObject(hBmAnd, sizeof(BITMAP), &bm);

    BitBlt(hMemDC,(w - bm.bmWidth) / 2, (h - bm.bmHeight - 70) / 2, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY); 

    SelectObject(hMemDC, GetStockObject(BLACK_BRUSH));
    Polygon(hMemDC, p, 4);
    SelectObject(hMemDC, GetStockObject(NULL_BRUSH));

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
    DeleteObject(hBmAnd);
    DeleteObject(hBmXor);
    DeleteDC(hMemDCLogo);
    PostQuitMessage(0);
    KillTimer(hWnd, 23);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of MyWindowFunc */

/* EOF 'T01EYES.C' */