/* FILE NAME: ANIM.H
 * PROGRAMMER: AK5
 * DATE: 14.06.17
 * PURPOSE: anim
 */

#ifndef ANIM_H
#define ANIM_H

#include "Def.h"
#include "Render.h"

#define AK5_MAX_UNITS 10000





typedef struct tagak5ANIM ak5ANIM;
typedef struct tagak5UNIT ak5UNIT;

#define AK5_UNIT_BASE_FIELDS \
  VOID (*Init) (ak5UNIT *Uni, ak5ANIM *Ani); \
  VOID (*Close) (ak5UNIT *Uni, ak5ANIM *Ani); \
  VOID (*Response) (ak5UNIT *Uni, ak5ANIM *Ani); \
  VOID (*Render) (ak5UNIT *Uni, ak5ANIM *Ani);

extern ak5ANIM AK5_Anim;
extern INT AK5_MouseWheel;

struct tagak5ANIM
{
  HWND hWnd;                    
  HDC hDC;      /* Drawing window context */
  HGLRC hGLRC;  /* Rendering context */
  INT W, H;                  
  ak5UNIT *Units[AK5_MAX_UNITS];
  INT NumOfUnits;
  DBL GlobalTime, GlobalDeltaTime, Time, DeltaTime, FPS;
  BOOL IsPause;
  BYTE Keys[256];
  BYTE KeysOld[256];
  BYTE KeysClick[256];
  INT
    Mx, My, Mz,     /* Absolute coordinates */
    Mdx, Mdy, Mdz;  /* Relative coordinates shift */
    BYTE
    JBut[32],                    /* Joystick button state */
    JButOld[32],                 /* Joystick button old state */
    JButClick[32];               /* Joystick button click info */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL
    Jx, Jy, Jz, Jr;              /* Joystick axes */
};

struct tagak5UNIT
{
  AK5_UNIT_BASE_FIELDS
};

BOOL AK5_AnimInit( HWND hWnd );
VOID AK5_AnimClose( VOID );
VOID AK5_AnimResize( INT w, INT h );
VOID AK5_AnimCopyFrame( VOID );
VOID AK5_AnimRender( VOID );
VOID AK5_AnimAddUnit( ak5UNIT *Uni );
VOID AK5_AnimDoExit( VOID );
VOID AK5_AnimFlipFullScreen( VOID );

#endif

/* EOF 'ANIM.H' */