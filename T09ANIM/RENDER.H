/* FILE NAME: RENDER.H
 * PROGRAMMER: AK5
 * DATE: 14.06.17
 * PURPOSE: Base rendering system module.
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

/* OpenGL specific includes */
#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Project parameters */
extern DBL
  AK5_RndProjFarClip,
  AK5_RndProjDist, /* Distance from viewer to project plane */
  AK5_RndProjSize; /* Prohect plane inner size */

extern MATR
  AK5_RndMatrProj,  
  AK5_RndMatrView; /* Viewer matrix */

extern UINT
  AK5_RndProgId;

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
typedef struct tagak5VERTEX
{
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
} ak5VERTEX;

typedef struct tagak5PRIM
{
  BOOL IsTrimesh;
  INT NumOfI;
  MATR M;
  INT VA, VBuf;
  INT IBuf;
} ak5PRIM;

typedef struct tagak5OBJ
{
  ak5PRIM *P;
  INT NumOfP;
} ak5OBJ;

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AK5_RndPrimLoad( ak5PRIM *Obj, CHAR *FileName );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ *Obj;
 * RETURNS: None.
 */
VOID AK5_RndObjFree( ak5OBJ *Obj );

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID AK5_RndObjDraw( ak5OBJ *Obj, MATR M );

VOID AK5_RndPrimCreate( ak5PRIM *Pr, BOOL IsTrimesh,
                        ak5VERTEX *V, INT NumOfV,
                        INT *I, INT NumOfI );

VOID AK5_RndPrimFree( ak5PRIM *Pr );

VOID AK5_RndPrimDraw( ak5PRIM *Pr, MATR M );

VOID AK5_RndTriMeshEvalNormals( ak5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );

VOID AK5_RndGridEvalNormals( ak5VERTEX *V, INT N, INT M );

VOID AK5_RndPrimCreateSphere( ak5PRIM *Pr, VEC C, FLT R, INT N, INT M );

VOID AK5_RndPrimCreatePlane( ak5PRIM *Pr, VEC C, VEC Du, VEC Dv, INT N, INT M );

VOID AK5_RndShaderFree( UINT Prg );

UINT AK5_RndShaderLoad( CHAR *FileNamePrefix );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
