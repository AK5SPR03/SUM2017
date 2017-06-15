/* FILE NAME: OBJ.C
 * PROGRAMMER: AK5
 * DATE: 14.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Create object function.
 * ARGUMENTS:
 *   - created object:
 *       ak5OBJ *Obj;
 *   - number of primitives:
 *       INT NumOfP;
* RETURNS: None.
 */
VOID AK5_RndObjCreate( ak5OBJ *Obj, INT NumOfP )
{
  memset(Obj, 0, sizeof(ak5PRIM));
  if ((Obj->P = malloc(sizeof(ak5PRIM) * NumOfP)) == NULL)
    return;
  memset(Obj->P, 0, sizeof(ak5PRIM) * NumOfP);
  Obj->NumOfP = NumOfP;
} /* Ens of 'AK5_RndObjCreate' function */

/* Free object function.
 * ARGUMENTS:
 *   - deleted object:
 *       ak5OBJ *Obj;
 * RETURNS: None.
 */
VOID AK5_RndObjFree( ak5OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    AK5_RndPrimFree(&Obj->P[i]);
  free(Obj->P);
  memset(Obj, 0, sizeof(ak5PRIM));
} /* Ens of 'AK5_RndObjFree' function */

/* Draw primitive function.
 * ARGUMENTS:
 *   - drawing object:
 *       ak5OBJ *Obj;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID AK5_RndObjDraw( ak5OBJ *Obj, MATR M )
{
  INT i;
  for (i = 0; i < Obj->NumOfP; i++)
    AK5_RndPrimDraw(&Obj->P[i], M);
} /* Ens of 'AK5_RndObjDraw' function */

/* END OF 'OBJ.C' FILE */