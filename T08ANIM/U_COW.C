/* FILE NAME: U_COW.C
 * PROGRAMMER: AK5
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

/* Animation unit representation type */


/* Cow drawing unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT_COW *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitInit( ak5UNIT_COW *Uni, ak5ANIM *Ani )
{
  AK5_RndObjLoad(&Uni->Cow, "cow.object");
} /* End of 'AK5_UnitInit' function */

/* Cow drawing unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT_COW *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitClose( ak5UNIT_COW *Uni, ak5ANIM *Ani )
{
  AK5_RndObjFree(&Uni->Cow);
} /* End of 'AK5_UnitClose' function */

/* Cow drawing unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT_COW *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitResponse( ak5UNIT_COW *Uni, ak5ANIM *Ani )
{
} /* End of 'AK5_UnitResponse' function */

/* Cow drawing unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT_COW *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitRender( ak5UNIT_COW *Uni, ak5ANIM *Ani )
{ 
  if (Uni->rot.x == 1 && Uni->rot.y == 1 && Uni->rot.z == 1) 
    Uni->rot = VecSet(rand() % 5, rand() % 5, rand() % 5);
  if (Uni->pos.x == 1 && Uni->pos.y == 1 && Uni->pos.z == 1) 
    Uni->pos = VecSet(rand() % 40 - 20, rand() % 20 - 10, rand() % 20 - 10);
  AK5_RndObjDraw(&Uni->Cow, MatrMulMatr(MatrRotate(Ani->Time * 102, Uni->rot), MatrTranslate(VecSet1((DBL)Ani->Mz / 500))));
} /* End of 'AK5_UnitRender' function */

/* Cow drawing unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ak5UNIT *) pointer to created unit.
 */
ak5UNIT * AK5_UnitCreateCow( VOID )
{
  ak5UNIT_COW *Uni;
  /* Memory allocation */
  if ((Uni = (ak5UNIT_COW *)AK5_AnimUnitCreate(sizeof(ak5UNIT_COW))) == NULL)
    return NULL;
  
  Uni->pos = VecSet1(1);
  Uni->rot = VecSet1(1);
  /* Setup unit methods */
  Uni->Init = (VOID *)AK5_UnitInit;
  Uni->Close = (VOID *)AK5_UnitClose;
  Uni->Response = (VOID *)AK5_UnitResponse;
  Uni->Render = (VOID *)AK5_UnitRender;
  return (ak5UNIT *)Uni;
} /* End of 'AK5_UnitCreateCow' function */

/* END OF 'U_COW.C' FILE */
