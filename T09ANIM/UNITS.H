/* FILE NAME: UNITS.H
 * PROGRAMMER: AK5
 * DATE: 14.06.17
 * PURPOSE: units
 */

#ifndef UNITS_H
#define UNITS_H

#include "Def.h"

#include "Anim.h"

typedef struct tagak5UNIT_COW
{
  AK5_UNIT_BASE_FIELDS
  ak5OBJ3D Cow; /* Cow model */
  VEC pos, rot;
  INT id, angle;
} ak5UNIT_COW;


ak5UNIT * AK5_UnitCreateCow( INT ident );
ak5UNIT * AK5_UnitCreateControl( VOID );
ak5UNIT * AK5_AnimUnitCreate( INT Size );
#endif

/* EOF 'UNITS.H' */