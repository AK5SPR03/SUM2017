/* FILE NAME: POLE.C
 * PROGRAMMER: AK5
 * DATE: 04.06.17
 * PURPOSE: Drawing magnetic arrows
 */

#include <math.h>

#include "Pole.h"

#define lr 10
#define br 50

/* Function DrawArrow */
VOID DrawArrow(HDC hDC, INT x, INT y, INT cx, INT cy)
{
  HBRUSH hBr;
  static POINT pB[3], pR[3], ptB[3], ptR[3];
  FLOAT mSin, mCos, hptn;
  
  /* set coordinates */
  hptn = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
  mSin = (cy - y) / hptn;
  mCos = (cx - x) / hptn;

  pB[0].x = x + br * mCos;
  pB[0].y = y + br * mSin;
  pB[1].x = x + lr * mSin;
  pB[1].y = y - lr * mCos;
  pB[2].x = x - lr * mSin;
  pB[2].y = y + lr * mCos;

  pR[0].x = x - br * mCos;
  pR[0].y = y - br * mSin;

  pR[1] = pB[1];
  pR[2] = pB[2];
  /* end setting coordinates */

  /* drawing */
  hBr = CreateSolidBrush(RGB(0, 0, 255));
  SelectObject(hDC, hBr);
  Polygon(hDC, pB, 3); 
  DeleteObject(hBr);
  hBr = CreateSolidBrush(RGB(255, 0, 0));
  SelectObject(hDC, hBr);
  Polygon(hDC, pR, 3);
  DeleteObject(hBr);
  /* end of drawing */

} /* End of DrawArrow */

/* EOF 'POLE.C' */