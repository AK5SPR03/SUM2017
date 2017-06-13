/* FILE NAME: ANIM.C
 * PROGRAMMER: AK5
 * DATE: 09.06.17
 * PURPOSE: anim
 */

#include "Anim.h"
#include "Units.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

/* Joystic axis value determination */
#define AK5_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min - 1) - 1)

ak5ANIM AK5_Anim;
INT AK5_MouseWheel;

static UINT64 AK5_StartTime, 
              AK5_OldTime,
              AK5_OldTimeFPS,
              AK5_PauseTime,
              AK5_TimePerSec,
              AK5_FrameCounter;

VOID AK5_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;

  memset(&AK5_Anim, 0, sizeof(ak5ANIM));
  AK5_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  AK5_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  QueryPerformanceFrequency(&t);
  AK5_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  AK5_StartTime = AK5_OldTime = AK5_OldTimeFPS = t.QuadPart;
  AK5_FrameCounter = 0;
  AK5_PauseTime = 0;

  AK5_Anim.IsPause = FALSE;
  AK5_Anim.FPS = 50;
  AK5_AnimAddUnit(AK5_UnitCreateControl());
  AK5_RndInit();

}

VOID AK5_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < AK5_Anim.NumOfUnits; i++)
  {
    AK5_Anim.Units[i]->Close(AK5_Anim.Units[i], &AK5_Anim);
    free(AK5_Anim.Units[i]);
  }
  AK5_Anim.NumOfUnits = 0;

  DeleteDC(AK5_Anim.hDC);
  DeleteObject(AK5_Anim.hFrame);

  memset(&AK5_Anim, 0, sizeof(ak5ANIM));

}

VOID AK5_AnimResize( INT w, INT h )
{
  HDC hDC;

  AK5_Anim.W = w;
  AK5_Anim.H = h;

  if (AK5_Anim.hFrame != NULL)
    DeleteObject(AK5_Anim.hFrame);
  hDC = GetDC(AK5_Anim.hWnd);
  AK5_Anim.hFrame = CreateCompatibleBitmap(hDC, w, h);
  ReleaseDC(AK5_Anim.hWnd, hDC);

  SelectObject(AK5_Anim.hDC, AK5_Anim.hFrame);
  AK5_RndSetProj();
}

VOID AK5_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, AK5_Anim.W, AK5_Anim.H, AK5_Anim.hDC, 0, 0, SRCCOPY);
}

VOID AK5_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER t;
  POINT pt;

  for (i = 0; i < AK5_Anim.NumOfUnits; i++)
    AK5_Anim.Units[i]->Response(AK5_Anim.Units[i], &AK5_Anim);
  SelectObject(AK5_Anim.hDC, GetStockObject(DC_PEN));
  SelectObject(AK5_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(AK5_Anim.hDC, RGB(10, 40, 20));
  SetDCPenColor(AK5_Anim.hDC, RGB(255, 0, 0));
  Rectangle(AK5_Anim.hDC, 0, 0, AK5_Anim.W + 1, AK5_Anim.H + 1);

  Rectangle(AK5_Anim.hDC, 0, 0, AK5_Anim.W + 1, AK5_Anim.H + 1);
  for (i = 0; i < AK5_Anim.NumOfUnits; i++)
  {
    SelectObject(AK5_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AK5_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AK5_Anim.hDC, RGB(0, 255, 255));
    SetDCPenColor(AK5_Anim.hDC, RGB(0, 250, 100));;
    

    AK5_Anim.Units[i]->Render(AK5_Anim.Units[i], &AK5_Anim);
  }


  /*** Handle timer ***/
  AK5_FrameCounter++;                    /* increment frame counter (for FPS) */
  QueryPerformanceCounter(&t);           /* obtain current timer value */
  /* Global time */
  AK5_Anim.GlobalTime = (DBL)(t.QuadPart - AK5_StartTime) / AK5_TimePerSec;
  AK5_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - AK5_OldTime) / AK5_TimePerSec;
  /* Time with pause */
  if (AK5_Anim.IsPause)
  {
    AK5_PauseTime += t.QuadPart - AK5_OldTime;
    AK5_Anim.DeltaTime = 0;
  }
  else
  {
    AK5_Anim.Time = (DBL)(t.QuadPart - AK5_PauseTime - AK5_StartTime) / AK5_TimePerSec;
    AK5_Anim.DeltaTime = AK5_Anim.GlobalDeltaTime;
  }
  /* FPS */
  if (t.QuadPart - AK5_OldTimeFPS > AK5_TimePerSec)
  {
    AK5_Anim.FPS = (DBL)AK5_FrameCounter * AK5_TimePerSec / (t.QuadPart - AK5_OldTimeFPS);
    AK5_OldTimeFPS = t.QuadPart;
    AK5_FrameCounter = 0;
  }
  AK5_OldTime = t.QuadPart;

  /*** Handle keyboard ***/

  GetKeyboardState(AK5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AK5_Anim.Keys[i] >>= 7;
    AK5_Anim.KeysClick[i] = AK5_Anim.Keys[i] && !AK5_Anim.KeysOld[i];
  }
  memcpy(AK5_Anim.KeysOld, AK5_Anim.Keys, 256);

  /*** Handle mouse ***/

  GetCursorPos(&pt);
  ScreenToClient(AK5_Anim.hWnd, &pt);
  AK5_Anim.Mdx = pt.x - AK5_Anim.Mx;
  AK5_Anim.Mdy = pt.y - AK5_Anim.My;
  AK5_Anim.Mx = pt.x;
  AK5_Anim.My = pt.y;

  /*** Handle wheel ***/

  AK5_Anim.Mdz = AK5_MouseWheel;
  AK5_Anim.Mz += AK5_MouseWheel;
  AK5_MouseWheel = 0;

  /*** Handle joystick ***/
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(JOYCAPS)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Axes */
        AK5_Anim.Jx = AK5_GET_JOYSTICK_AXIS(X);
        AK5_Anim.Jy = AK5_GET_JOYSTICK_AXIS(Y);
        AK5_Anim.Jz = AK5_GET_JOYSTICK_AXIS(Z);
        AK5_Anim.Jr = AK5_GET_JOYSTICK_AXIS(R);

        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          AK5_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          AK5_Anim.JButClick[i] = AK5_Anim.JBut[i] && !AK5_Anim.JButOld[i];
          memcpy(AK5_Anim.JButOld, AK5_Anim.JBut, 32);
        }

        /* Point of view */
        AK5_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }


}

VOID AK5_AnimAddUnit( ak5UNIT *Uni )
{
  if (AK5_Anim.NumOfUnits < AK5_MAX_UNITS)
  {
    AK5_Anim.Units[AK5_Anim.NumOfUnits] = Uni;
    AK5_Anim.Units[AK5_Anim.NumOfUnits]->Init(AK5_Anim.Units[AK5_Anim.NumOfUnits], &AK5_Anim);
    AK5_Anim.NumOfUnits++;
  }
}

VOID AK5_AnimDoExit( VOID )
{
  static BOOL isExit = FALSE;

  if (isExit)
    return;
  isExit = TRUE;
  PostMessage(AK5_Anim.hWnd, WM_CLOSE, 0, 0);
}

VOID AK5_AnimFlipFullScreen( VOID )
{
  static BOOL isFullScreen;
  static RECT SaveRect;
  if (isFullScreen)
  {
    SetWindowPos(AK5_Anim.hWnd, HWND_NOTOPMOST, 
      SaveRect.left, SaveRect.top, 
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top, 
      SWP_NOOWNERZORDER);
  }
  else
  {
    HMONITOR hMon;
    MONITORINFOEX moninfo;
    RECT rc;

    GetWindowRect(AK5_Anim.hWnd, &SaveRect);

    hMon = MonitorFromWindow(AK5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(AK5_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AK5_Anim.hWnd, HWND_NOTOPMOST, 
      rc.left, rc.top, 
      rc.right - rc.left, rc.bottom - rc.top, 
      SWP_NOOWNERZORDER);
  }
  isFullScreen = !isFullScreen;
}
/* EOF 'ANIM.C' */