/* FILE NAME: OBJ3D.C
 * PROGRAMMER: AK5
 * DATE: 10.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AK5_RndObjLoad( ak5OBJ3D *Obj, CHAR *FileName )
{
  INT vn = 0, fn = 0, size;
  FILE *F;
  static CHAR Buf[1000];

  memset(Obj, 0, sizeof(ak5OBJ3D));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;
  }

  /* Memory allocation */
  size = sizeof(VEC) * vn + sizeof(INT [3]) * fn;
  if ((Obj->V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Obj->F = (INT (*)[3])(Obj->V + vn);
  Obj->NumOfV = vn;
  Obj->NumOfF = fn;

  /* Read model data */
  rewind(F);
  vn = 0;
  fn = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Obj->V[vn++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n0, n1, n2;

      if (sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i %i %i", &n0, &n1, &n2) == 3)
      {
        Obj->F[fn][0] = n0 - 1;
        Obj->F[fn][1] = n1 - 1;
        Obj->F[fn][2] = n2 - 1;
        fn++;
      }
    }
  }

  fclose(F);
  return TRUE;
} /* End of 'AK5_RndObjLoad' function */

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ3D *Obj;
 * RETURNS: None.
 */
VOID AK5_RndObjFree( ak5OBJ3D *Obj )
{
  if (Obj->V != NULL)
    free(Obj->V);
  memset(Obj, 0, sizeof(ak5OBJ3D));
} /* End of 'AK5_RndObjFree function */

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       ak5OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID AK5_RndObjDraw( ak5OBJ3D *Obj, MATR M )
{
  INT i;
  POINT *pts;
  MATR WVP = MatrMulMatr(M, MatrMulMatr(AK5_RndMatrView, AK5_RndMatrProj));

  if ((pts = malloc(sizeof(POINT) * Obj->NumOfV)) == NULL)
    return;


  /* Project all points */
  for (i = 0; i < Obj->NumOfV; i++)
  {
    VEC pp = PointTransform(Obj->V[i], WVP);

    pts[i].x = AK5_Anim.W / 2 * (pp.x + 1);
    pts[i].y = AK5_Anim.H / 2 * (-pp.y + 1);
  }

  /* Draw all facets */
  for (i = 0; i < Obj->NumOfF; i++)
  {
    POINT *p = &pts[Obj->F[i][0]];
    MoveToEx(AK5_Anim.hDC, p->x, p->y, NULL);

    p = &pts[Obj->F[i][1]];
    LineTo(AK5_Anim.hDC, p->x, p->y);

    p = &pts[Obj->F[i][2]];
    LineTo(AK5_Anim.hDC, p->x, p->y);

    p = &pts[Obj->F[i][0]];
    LineTo(AK5_Anim.hDC, p->x, p->y);
  }
  free(pts);
} /* End of 'AK5_RndObjDraw' function */

/* END OF 'OBJ3D.C' FILE */
