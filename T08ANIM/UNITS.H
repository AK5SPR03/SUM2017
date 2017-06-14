/* FILE NAME: UNITS.H
 * PROGRAMMER: AK5
 * DATE: 09.06.17
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
} ak5UNIT_COW;


ak5UNIT * AK5_UnitCreateCow( VOID );
ak5UNIT * AK5_UnitCreateControl( VOID );
ak5UNIT * AK5_AnimUnitCreate( INT Size );
#endif

/* EOF 'UNITS.H' */