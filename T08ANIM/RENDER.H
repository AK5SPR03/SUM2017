/* FILE NAME: RENDER.H
 * PROGRAMMER: AK5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

/* Project parameters */
extern DBL
  AK5_RndProjFarClip,
  AK5_RndProjDist, /* Distance from viewer to project plane */
  AK5_RndProjSize; /* Prohect plane inner size */

extern MATR
  AK5_RndMatrProj,  
  AK5_RndMatrView; /* Viewer matrix */

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK5_RndInit( VOID );

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK5_RndSetProj( VOID );

/***
 * Object handle
 ***/

/* Object description type */
typedef struct tagak5OBJ3D
{
  VEC *V;      /* Vertex array */
  INT NumOfV;  /* Vertex array size */
  INT (*F)[3]; /* Facets array (point indices) */
  INT NumOfF;  /* Facets array size */
} ak5OBJ3D;

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AK5_RndObjLoad( ak5OBJ3D *Obj, CHAR *FileName );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ3D *Obj;
 * RETURNS: None.
 */
VOID AK5_RndObjFree( ak5OBJ3D *Obj );

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID AK5_RndObjDraw( ak5OBJ3D *Obj, MATR M );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
