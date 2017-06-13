/* FILE NAME: RENDER.C
 * PROGRAMMER: AK5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#include "anim.h"

/* Project parameters */
DBL
  AK5_RndWp,       /* Project plane width */
  AK5_RndHp,       /* Project plane height */
  AK5_RndProjDist, /* Distance from viewer to project plane */
  AK5_RndProjSize; /* Prohect plane inner size */

MATR
  AK5_RndMatrView; /* Viewer matrix */

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK5_RndInit( VOID )
{
  AK5_RndWp = 1;
  AK5_RndHp = 1;
  AK5_RndProjDist = 1;
  AK5_RndProjSize = 1;

  AK5_RndMatrView = MatrView(VecSet1(23), VecSet1(6), VecSet(0, 1, 0));
} /* End of 'AK5_RndInit' function */

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK5_RndSetProj( VOID )
{
  AK5_RndWp = AK5_RndProjSize;
  AK5_RndHp = AK5_RndProjSize;
  if (AK5_Anim.W > AK5_Anim.H)
    AK5_RndWp *= (DBL)AK5_Anim.W / AK5_Anim.H;
  else
    AK5_RndHp *= (DBL)AK5_Anim.H / AK5_Anim.W;
} /* End of 'AK5_RndSetProj' function */

/* END OF 'RENDER.C' FILE */
