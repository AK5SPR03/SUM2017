/* FILE NAME: SPHERE.C
 * PROGRAMMER: AK5
 * DATE: 07.06.17
 * PURPOSE: Drawing sphere
 */

#include <math.h>
#include <time.h>
#include "Sphere.h"

#define M 25
#define N 50

static VEC G[M][N];
static POINT Pts[M][N];

/* function sin1 */
DBL sin1( DBL a, DBL p )
{
  return (sin(a) > 0)? pow(sin(a), p) : -pow(-sin(a), p);
} /* End of sin1 */

/* function cos1 */
DBL cos1( DBL a, DBL p )
{
  return (cos(a) > 0)? pow(cos(a), p) : -pow(-cos(a), p);
} /* End of cos1 */

/* function SphereBuild */
VOID SphereBuild( DBL r )
{
  INT i, j;
  DBL thet, phi;

  for (thet = 0, i = 0; i < M; i++, thet += PI / (M - 1))
    for (phi = 0, j = 0; j < N; j++, phi += 2 * PI / (N - 1))
    {
      G[i][j].x = r * sin1(thet, 1) * sin1(phi, 1);
      G[i][j].y = r * cos1(thet, 1);
      G[i][j].z = r * sin1(thet, 1) * cos1(phi, 1);
    }
} /* End of SphereBuild */

/* function MatrRotate */
MATR MatrRotate( VEC p, DBL AngleDeg )
{
  DBL a = AngleDeg / 180 * PI, sine = sin(a), cosine = cos(a);
  MATR m;

  a = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  p.x /= a;
  p.y /= a;
  p.z /= a;
  
  m.a[0][0] = cosine + p.x * p.x * (1 - cosine);
  m.a[0][1] = p.y * p.x * (1 - cosine) + p.z * sine;
  m.a[0][2] = p.z * p.x * (1 - cosine) - p.y * sine;

  m.a[1][0] = p.x * p.y * (1 - cosine) - p.z * sine;
  m.a[1][1] = cosine + p.y * p.y * (1 - cosine);
  m.a[1][2] = p.z * p.y * (1 - cosine) + p.x * sine;

  m.a[2][0] = p.x * p.z * (1 - cosine) + p.y * sine;
  m.a[2][1] = p.y * p.z * (1 - cosine) - p.x * sine;
  m.a[2][2] = cosine + p.z * p.z * (1 -cosine);
  
  return m;
} /* End of MatrRotate */

/* function VecTransform */
VEC VecTransform( VEC V, MATR Matr )
{
  VEC P =
  {
    V.x * Matr.a[0][0] + V.y * Matr.a[0][1] + V.z * Matr.a[0][2],
    V.x * Matr.a[1][0] + V.y * Matr.a[1][1] + V.z * Matr.a[1][2],
    V.x * Matr.a[2][0] + V.y * Matr.a[2][1] + V.z * Matr.a[2][2]
  };

  return P;
} /* End of VecTransform */

/* function SphereDraw */
VOID SphereDraw( HDC hDC, INT w, INT h )
{
  INT i, j;
  DBL size = 10, wp = size, hp = size;
  static DBL t;

  t = clock() / (DBL)CLOCKS_PER_SEC;

  if (w > h)
    wp *= (DBL)w / h;
  else
    hp *= (DBL)h / w;

  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
  SelectObject(hDC, GetStockObject(WHITE_PEN));
  SetDCPenColor(hDC, RGB(255, 25, 255));
  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++)
    {
      DBL xp, yp;
      VEC os = {5, 2, 0}, rez;
      POINT tmp[4];
      

      rez = VecTransform(G[i][j], MatrRotate(os, t * 10));
      xp = rez.x;
      yp = rez.y;
      Pts[i][j].x = xp * w / wp + 0.5 * w;
      Pts[i][j].y = 0.5 * h - yp * h / hp;
      //Ellipse(hDC, Pts[i][j].x - 2, Pts[i][j].y - 2, Pts[i][j].x + 2, Pts[i][j].y + 2);

      if (i > 0 && j > 0)
      {
        DBL s;
        tmp[0] = Pts[i][j];
        tmp[1] = Pts[i - 1][j];
        tmp[2] = Pts[i - 1][j - 1];
        tmp[3] = Pts[i][j - 1];

        s = (tmp[0].x - tmp[1].x) * (tmp[0].y + tmp[1].y) + 
            (tmp[1].x - tmp[2].x) * (tmp[1].y + tmp[2].y) + 
            (tmp[2].x - tmp[3].x) * (tmp[2].y + tmp[3].y) + 
            (tmp[3].x - tmp[0].x) * (tmp[3].y + tmp[0].y);
        SetDCPenColor(hDC, RGB(255, 25, 255));
        SelectObject(hDC, GetStockObject(NULL_BRUSH));
        if (s > 0)
          Polygon(hDC, tmp, 4);
        SelectObject(hDC, GetStockObject(WHITE_BRUSH));
      }
    }

  SetDCPenColor(hDC, RGB(255, 25, 255));
  SelectObject(hDC, GetStockObject(NULL_BRUSH));
    


} /* End of SphereDrawd */

/* EOF 'SPHERE.C' */