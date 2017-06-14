/* FILE NAME: UNIT.C
 * PROGRAMMER: AK5
 * DATE: 14.06.17
 * PURPOSE: Units
 */

#include "Anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitInit( ak5UNIT *Uni, ak5ANIM *Ani )
{
} /* End of 'AK5_UnitInit' function */
/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitClose( ak5UNIT *Uni, ak5ANIM *Ani )
{
} /* End of 'AK5_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitResponse( ak5UNIT *Uni, ak5ANIM *Ani )
{
} /* End of 'AK5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak5UNIT *Uni;
 *   - animation context:
 *       ak5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK5_UnitRender( ak5UNIT *Uni, ak5ANIM *Ani )
{
} /* End of 'AK5_UnitRender' function */


/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (ak5UNIT *) pointer to created unit.
 */
ak5UNIT * AK5_AnimUnitCreate( INT Size )
{
  ak5UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(ak5UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = AK5_UnitInit;
  Uni->Close = AK5_UnitClose;
  Uni->Response = AK5_UnitResponse;
  Uni->Render = AK5_UnitRender;
  return Uni;
} /* End of 'AK5_AnimUnitCreate' function */

/* EOF 'UNITS.C' */