/* FILE NAME: U_CONTR.C
 * PROGRAMMER: AK5
 * DATE: 14.06.17
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

/* Animation unit representation type */
typedef struct tagak5UNIT_CONTROL
{
  AK5_UNIT_BASE_FIELDS
} ak5UNIT_CONTROL;

/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT_CONTROL *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitInit( ak5UNIT_CONTROL *Uni, ak5ANIM *Ani )
{
  
} /* End of 'AK5_UnitInit' function */

/* Control unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT_CONTROL *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitClose( ak5UNIT_CONTROL *Uni, ak5ANIM *Ani )
{
} /* End of 'AK5_UnitClose' function */

/* Control unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT_CONTROL *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitResponse( ak5UNIT_CONTROL *Uni, ak5ANIM *Ani )
{
  static INT cowCounter = 0;

  if (Ani->KeysClick[VK_ESCAPE])
    AK5_AnimDoExit();
  else if (Ani->KeysClick['F'])
    AK5_AnimFlipFullScreen();
  else if (Ani->KeysClick['P'])
    AK5_Anim.IsPause = !AK5_Anim.IsPause;
  else if (Ani->KeysClick['C'])
  {
    AK5_AnimAddUnit(AK5_UnitCreateCow(cowCounter++));
  }
} /* End of 'AK5_UnitResponse' function */

/* Control unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT_CONTROL *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitRender( ak5UNIT_CONTROL *Uni, ak5ANIM *Ani )
{
  INT len;
  static CHAR Buf[100];

  len = sprintf(Buf, "FPS: %.5f, Units: %d, Wheel: %d"
    ", J: %.3f %.3f %.3f %.3f",
    Ani->FPS, Ani->NumOfUnits, Ani->Mz,
    Ani->Jx, Ani->Jy, Ani->Jz, Ani->Jr);

  SetWindowText(Ani->hWnd, Buf);
} /* End of 'AK5_UnitRender' function */

/* Control unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ak5UNIT *) pointer to created unit.
 */
ak5UNIT * AK5_UnitCreateControl( VOID )
{
  ak5UNIT_CONTROL *Uni;

  /* Memory allocation */
  if ((Uni = (ak5UNIT_CONTROL *)AK5_AnimUnitCreate(sizeof(ak5UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AK5_UnitInit;
  Uni->Close = (VOID *)AK5_UnitClose;
  Uni->Response = (VOID *)AK5_UnitResponse;
  Uni->Render = (VOID *)AK5_UnitRender;
  return (ak5UNIT *)Uni;
} /* End of 'AK5_UnitCreateControl' function */

/* END OF 'U_CONTR.C' FILE */
