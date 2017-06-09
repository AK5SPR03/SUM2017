/* FILE NAME: SPHERE.H
 * PROGRAMMER: AK5
 * DATE: 07.06.17
 * PURPOSE: Drawing sphere
 */

#ifndef SPFERE_H
#define SPFERE_H

#include <windows.h>

#pragma warning(disable: 4244)

#define PI 3.14159265358979323846

typedef double DBL;
typedef struct tagMATR
{
  DBL a[3][3];
} MATR;

typedef struct tagVEC
{
  DBL x, y, z;
} VEC;

VOID SphereBuild( DBL r );
VOID SphereDraw( HDC hDC, INT w, INT h );

#endif
/* EOF 'SPHERE.H' */