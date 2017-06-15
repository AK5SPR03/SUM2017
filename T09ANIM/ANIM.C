/* FILE NAME: ANIM.C
 * PROGRAMMER: AK5
 * DATE: 14.06.17
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

BOOL AK5_AnimInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};
  LARGE_INTEGER t;

  memset(&AK5_Anim, 0, sizeof(ak5ANIM));

  QueryPerformanceFrequency(&t);
  AK5_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  AK5_StartTime = AK5_OldTime = AK5_OldTimeFPS = t.QuadPart;
  AK5_FrameCounter = 0;
  AK5_PauseTime = 0;

  AK5_Anim.IsPause = FALSE;
  AK5_Anim.FPS = 50;
  AK5_AnimAddUnit(AK5_UnitCreateControl());

  /* Store window and create memory device context */
  AK5_Anim.hWnd = hWnd;
  AK5_Anim.hDC = GetDC(hWnd);

  /* OpenGL initialization: setup pixel format */
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AK5_Anim.hDC, &pfd);
  DescribePixelFormat(AK5_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AK5_Anim.hDC, i, &pfd);

  /* OpenGL initialization: setup rendering context */
  AK5_Anim.hGLRC = wglCreateContext(AK5_Anim.hDC);
  wglMakeCurrent(AK5_Anim.hDC, AK5_Anim.hGLRC);
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AK5_Anim.hGLRC);
    ReleaseDC(AK5_Anim.hWnd, AK5_Anim.hDC);
    return FALSE;
  }

  AK5_RndProgId = AK5_RndShaderLoad("A");

  AK5_RndInit();
  return TRUE;
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

  /* Delete OpenGL data */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AK5_Anim.hGLRC);
  /* Delete GDI data */
  ReleaseDC(AK5_Anim.hWnd, AK5_Anim.hDC);

  memset(&AK5_Anim, 0, sizeof(ak5ANIM));

  AK5_RndShaderFree(AK5_RndProgId);

}

VOID AK5_AnimResize( INT w, INT h )
{
  AK5_Anim.W = w;
  AK5_Anim.H = h;

  glViewport(0, 0, w, h);

  AK5_RndSetProj();
}

VOID AK5_AnimCopyFrame( VOID )
{
  SwapBuffers(AK5_Anim.hDC);
}

VOID AK5_AnimRender( VOID )
{
  INT i;
  static DBL ShdTime;
  LARGE_INTEGER t;
  POINT pt;

  for (i = 0; i < AK5_Anim.NumOfUnits; i++)
    AK5_Anim.Units[i]->Response(AK5_Anim.Units[i], &AK5_Anim);

    /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    /*** Update shader ***/
  if (AK5_Anim.GlobalTime - ShdTime > 2)
  {
    AK5_RndShaderFree(AK5_RndProgId);
    AK5_RndProgId = AK5_RndShaderLoad("A");
    ShdTime = AK5_Anim.GlobalTime;
  }

  glFinish();
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