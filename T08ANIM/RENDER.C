/* FILE NAME: RENDER.C
 * PROGRAMMER: AK5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#include "anim.h"

/* Project parameters */
DBL
  AK5_RndProjFarClip,
  AK5_RndProjDist, /* Distance from viewer to project plane */
  AK5_RndProjSize; /* Prohect plane inner size */

MATR
  AK5_RndMatrView,  /* Viewer matrix */
  AK5_RndMatrProj;

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK5_RndInit( VOID )
{
  AK5_RndProjFarClip = 1000;
  AK5_RndProjDist = 1;
  AK5_RndProjSize = 1;

  AK5_RndMatrView = MatrView(VecSet1(23), VecSet1(0), VecSet(0, 1, 0));
} /* End of 'AK5_RndInit' function */

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK5_RndSetProj( VOID )
{
  DBL rx = AK5_RndProjSize / 2, ry = AK5_RndProjSize / 2;
  if (AK5_Anim.W > AK5_Anim.H)
    rx *= (DBL)AK5_Anim.W / AK5_Anim.H;
  else
    ry *= (DBL)AK5_Anim.H / AK5_Anim.W;
  AK5_RndMatrProj = MatrFrustum(-rx, rx, -ry, ry, AK5_RndProjDist, AK5_RndProjFarClip);
} /* End of 'AK5_RndSetProj' function */

/* END OF 'RENDER.C' FILE */
