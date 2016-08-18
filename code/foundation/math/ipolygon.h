#ifndef _IPOLYGON_H_
#define _IPOLYGON_H_

#include "foundation/itype.h"
#include "foundation/core/imetatypes.h"
#include "foundation/container/islice.h"
#include "foundation/util/iarraytypes.h"

#include "foundation/math/ipos.h"
#include "foundation/math/irect.h"
#include "foundation/math/iplane.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
    
/*************************************************************/
/* ipolygon3d                                                */
/*************************************************************/

/* polygon 3d definition */
typedef struct ipolygon3d {
    irefdeclare;
    
    /*ipos3 slice*/
    islice *pos;
    /* max point */
    ipos3 max;
    /* min point */
    ipos3 min;
    /* center point */
    ipos3 center;
    
    /* accumulating the pos */
    ipos3 accumulating;
    
    /* the polygon plane */
    iplane plane;
}ipolygon3d;
    
/* destructor */
void ipolygon3d_destructor(ithis x, iobj *o);

/* create a polygon 3d*/
ipolygon3d *ipolygon3dmake(size_t capacity);

/* free a polygon 3d*/
void ipolygon3dfree(ipolygon3d *);

/* add ivec3 to polygon*/
void ipolygon3dadd(ipolygon3d *poly, const ipos3 *v, int nums);

/* caculating the center of polygon3d  */
void ipolygon3dfinish(ipolygon3d *poly);

/* take the polygon3d as a wrap buffer of pos */
const ipos3 * ipolygon3dpos3(ipolygon3d *polygon, int index);

/* take the polygon3d pos (x, z) as a wrap buffer of pos */
ipos ipolygon3dposxz(ipolygon3d *polygon, int index);

/* the the edge (center-middle) point*/
ipos3 ipolygon3dedgecenter(ipolygon3d *polygon, int index);

/* if the point in polygon, just like 2d contains*/
/* Left Hand System
 * y     z
 * ^     ^
 * |    /
 * |   /
 * |  /
 * | /
 * |---------> x
 * */
int ipolygon3dincollum(const ipolygon3d *poly, const ipos3 *v);

/* give the projection rect in xz-plane */
void ipolygon3dtakerectxz(const ipolygon3d *poly, irect *r);

/*************************************************************/
/* ipolygon2d                                                */
/*************************************************************/

/* polygon 2d definition */
typedef struct ipolygon2d {
    irefdeclare;
    
    /*ivec2 slice*/
    islice *slice;
    ivec2 max;
    ivec2 min;
}ipolygon2d;
    
/* destructor */
void ipolygon2d_destructor(ithis x, iobj *o);

/* create a polygon 2d*/
ipolygon2d *ipolygon2dmake(size_t capacity);

/* free a polygon 2d*/
void ipolygon2dfree(ipolygon2d *);

/* add ivec2 to polygon*/
void ipolygon2dadd(ipolygon2d *poly, const ivec2 *v, int nums);

/* if the point in polygon*/
int ipolygon2dcontains(const ipolygon2d *poly, const ivec2 *v);
    
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* _IPOLYGON_H_ */