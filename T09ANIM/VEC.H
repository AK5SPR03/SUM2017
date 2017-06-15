/* FILE NAME: VEC.H
 * PROGRAMMER: AK5
 * DATE: 14.06.17
 * PURPOSE: Library
 */

#ifndef VEC_H
#define VEC_H

#include <math.h>
#include <windows.h> 
                 
#pragma warning(disable: 4244)

#define PI 3.14159265358979323846
#define D2R(A) (A * PI / 180)
#define Degree2Radian(a) D2R(a)

typedef double DBL;
typedef float FLT;
typedef struct tagMATR
{
  FLT a[4][4];
} MATR;

typedef struct tagVEC
{
  FLT x, y, z;
} VEC;

typedef struct tagVEC2
{
  FLT x, y;
} VEC2;

typedef struct tagVEC4
{
  FLT x, y, z, w;
} VEC4;

__inline FLT Rnd1()
{
  return sin(rand());
}


__inline VEC VecSet( FLT x, FLT y, FLT z )
{
  VEC v = {x, y, z};
  return v;
}

__inline VEC VecSet1( FLT x )
{
  return VecSet(x, x, x);
}

__inline VEC2 Vec2Set( FLT x, FLT y )
{
  VEC2 v = {x, y};
  return v;
}

__inline VEC2 Vec2Set1( FLT x )
{
  return Vec2Set(x, x);
}

__inline VEC4 Vec4Set( FLT x, FLT y, FLT z, FLT w )
{
  VEC4 v = {x, y, z, w};
  return v;
}

__inline VEC4 Vec4Set1( FLT x )
{
  return Vec4Set(x, x, x, x);
}

__inline VEC VecAddVec( VEC v1, VEC v2 )
{
  return VecSet(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

__inline VEC VecSubVec( VEC v1, VEC v2 )
{
  return VecSet(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

__inline VEC VecMulNum( VEC v, DBL n )
{
  return VecSet(v.x * n, v.y * n, v.z * n);
}

__inline VEC VecDivNum( VEC v, DBL n )
{
  return VecSet(v.x / n, v.y / n, v.z / n);
}

__inline VEC VecNeg( VEC v )
{
  return VecSet(-v.x, -v.y, -v.z );
}

__inline FLT VecDotVec( VEC v1, VEC v2 )
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static MATR UnitMatrix = 
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}

__inline DBL VecLen2( VEC v )
{
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

__inline DBL VecLen( VEC v )
{
  return sqrt(VecLen2(v));
}

__inline VEC VecNormalize( VEC v )
{
  DBL l = VecLen(v);

  return VecSet(v.x / l, v.y / l, v.z / l);
}

__inline MATR MatrMulMatr( MATR m1, MATR m2 )
{
  INT i, j, k;
  MATR r;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      for(r.a[i][j] = 0, k = 0; k < 4; k++)
        r.a[i][j] += m1.a[i][k] * m2.a[k][j];
  return r;
}

#define MatrMulMatr3(A, B, C) \
  (MatrMulMatr(A, MatrMulMatr(B, C)))
#define MatrMulMatr4(A, B, C, D) \
  (MatrMulMatr(A, MatrMulMatr3(B, C, D)))

__inline MATR MatrTranspose( MATR m )
{
  INT i, j;
  MATR r;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      r.a[i][j] = r.a[j][i];
  return r;
}

__inline FLT MatrDeterm3x3( DBL a11, DBL a12, DBL a13,
                            DBL a21, DBL a22, DBL a23,
                            DBL a31, DBL a32, DBL a33)
{
  return a11 * a22 * a33 - a11 * a23 * a32 - a12 * a21 * a33 + 
         a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31;
}

__inline FLT MatrDeterm( MATR m )
{
  return
    m.a[0][0] * MatrDeterm3x3(m.a[1][1], m.a[1][2], m.a[1][3], 
                              m.a[2][1], m.a[2][2], m.a[2][3], 
                              m.a[3][1], m.a[3][2], m.a[3][3]) - 
    m.a[0][1] * MatrDeterm3x3(m.a[1][0], m.a[1][2], m.a[1][3], 
                              m.a[2][0], m.a[2][2], m.a[2][3], 
                              m.a[3][0], m.a[3][2], m.a[3][3]) +
    m.a[0][2] * MatrDeterm3x3(m.a[1][0], m.a[1][1], m.a[1][3], 
                              m.a[2][0], m.a[2][1], m.a[2][3], 
                              m.a[3][0], m.a[3][1], m.a[3][3]) - 
    m.a[0][3] * MatrDeterm3x3(m.a[1][0], m.a[1][1], m.a[1][1], 
                              m.a[2][0], m.a[2][1], m.a[2][1], 
                              m.a[3][0], m.a[3][1], m.a[3][1]);
}

__inline MATR MatrInverse( MATR m )
{
  INT i, j, p[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
  FLT det = MatrDeterm(m);
  MATR r;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      r.a[i][j] = 
        MatrDeterm3x3(
          m.a[p[i][0]][p[j][0]], m.a[p[i][0]][p[j][1]], m.a[p[i][0]][p[j][2]], 
          m.a[p[i][1]][p[j][0]], m.a[p[i][1]][p[j][1]], m.a[p[i][1]][p[j][2]], 
          m.a[p[i][2]][p[j][0]], m.a[p[i][2]][p[j][1]], m.a[p[i][2]][p[j][2]]) / 
        det;
  return r;
}

/* function MatrRotate */
__inline MATR MatrRotate(FLT AngleDeg, VEC v )
{
  FLT a = D2R(AngleDeg), si = sin(a), co = cos(a);
  MATR m = UnitMatrix;
  VEC p = VecNormalize(v);
  
  m.a[0][0] = co + p.x * p.x * (1 - co);
  m.a[0][1] = p.y * p.x * (1 - co) + p.z * si;
  m.a[0][2] = p.z * p.x * (1 - co) - p.y * si;

  m.a[1][0] = p.x * p.y * (1 - co) - p.z * si;
  m.a[1][1] = co + p.y * p.y * (1 - co);
  m.a[1][2] = p.z * p.y * (1 - co) + p.x * si;

  m.a[2][0] = p.x * p.z * (1 - co) + p.y * si;
  m.a[2][1] = p.y * p.z * (1 - co) - p.x * si;
  m.a[2][2] = co + p.z * p.z * (1 -co);
  
  return m;
} /* End of MatrRotate */

__inline MATR MatrTranslate( VEC t )
{
  MATR m = UnitMatrix;

  m.a[3][0] = t.x;
  m.a[3][1] = t.y;
  m.a[3][2] = t.z;
  return m;
}

__inline MATR MatrScale( VEC s )
{
  MATR m = UnitMatrix;

  m.a[0][0] = s.x;
  m.a[1][1] = s.y;
  m.a[2][2] = s.z;
  return m;
}

__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a = D2R(AngleInDegree);
  DBL c = cos(a), s = sin(a);

  m.a[0][0] = c;
  m.a[0][1] = s;
  m.a[1][0] = -s;
  m.a[1][1] = c;
  return m;
}

__inline MATR MatrRotateX( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a = D2R(AngleInDegree);
  DBL c = cos(a), s = sin(a);

  m.a[1][1] = c;
  m.a[1][2] = s;
  m.a[2][1] = -s;
  m.a[2][2] = c;
  return m;
}

__inline MATR MatrRotateY( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a = D2R(AngleInDegree);
  DBL c = cos(a), s = sin(a);

  m.a[0][0] = c;
  m.a[0][2] = -s;
  m.a[2][0] = s;
  m.a[2][2] = c;
  return m;
}

//__inline VEC VecMulMatr( VEC v, MATR m )
//{
//
//}



__inline VEC VecCrossVec( VEC v1, VEC v2 )
{
  return VecSet(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

/* function MatrView */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {               Right.x,                Up.x,              -Dir.x, 0},
      {               Right.y,                Up.y,              -Dir.y, 0},
      {               Right.z,                Up.z,              -Dir.z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* function VecMulMatr43 */
__inline VEC VecMulMatr43( VEC V, MATR M )
{
  return VecSet(V.x * M.a[0][0] + V.y * M.a[1][0] + V.z * M.a[2][0] + M.a[3][0],
                V.x * M.a[0][1] + V.y * M.a[1][1] + V.z * M.a[2][1] + M.a[3][1],
                V.x * M.a[0][2] + V.y * M.a[1][2] + V.z * M.a[2][2] + M.a[3][2]);
} /* End of 'VecMulMatr43' function */

/* function VecMulMatr3 */
__inline VEC VecMulMatr3( VEC V, MATR M )
{
  return VecSet(V.x * M.a[0][0] + V.y * M.a[1][0] + V.z * M.a[2][0],
                V.x * M.a[0][1] + V.y * M.a[1][1] + V.z * M.a[2][1],
                V.x * M.a[0][2] + V.y * M.a[1][2] + V.z * M.a[2][2]);
} /* End of 'VecMulMatr3' function */

/* function PointTransform */
__inline VEC PointTransform( VEC V, MATR M )
{
  DBL w = V.x * M.a[0][3] + V.y * M.a[1][3] + V.z * M.a[2][3] + M.a[3][3];

  return VecSet((V.x * M.a[0][0] + V.y * M.a[1][0] + V.z * M.a[2][0] + M.a[3][0]) / w,
                (V.x * M.a[0][1] + V.y * M.a[1][1] + V.z * M.a[2][1] + M.a[3][1]) / w,
                (V.x * M.a[0][2] + V.y * M.a[1][2] + V.z * M.a[2][2] + M.a[3][2]) / w);
} /* End of 'PointTransform' function */

/* function MatrFrustum */
__inline MATR MatrFrustum( FLT l, FLT r, FLT b, FLT t, FLT n, FLT f )
{
  MATR m = MatrIdentity();
  m.a[0][0] = 2 * n / (r - l);
  m.a[1][1] = 2 * n / (t - b);
  m.a[2][0] = (r + l) / (r - l);
  m.a[2][1] = (t + b) / (t - b);
  m.a[2][2] = -(f + n) / (f - n);
  m.a[2][3] = -1;
  m.a[3][2] = -2 * n * f / (f - n);
  m.a[3][3] = 0;
  return m;
}/* End of 'MatrFrustum' function */

#endif
/* EOF 'VEC.H' */