#ifndef _IVAR_H_
#define _IVAR_H_

#include "foundation/itype.h"
#include "foundation/core/iref.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* forward -declares */
struct imeta;
   
/* the ivar wrap all couldbeable */
typedef struct ivar {
    /* the &v is the real value of meta-funcs,
     * supposed we are in little-endian 
     * https://en.wikipedia.org/wiki/Endianness */
    union {
        int i;
        int64_t i64;
        uint64_t u64;
        ireal real;
        void *ptr;
        ipod pod;
        
        iref *ref;
    } v ; 
    const struct imeta *meta; /* type-meta */
} ivar;
    
/* ivar type: meta-index */
int ivartype(const ivar *var);
/* ivar type: meta */
const struct imeta *ivarmeta(const ivar *var);
    
/* simple ivar */
int ivarissimple(const ivar *var);
    
/* ivar is right-meta-type */
ibool ivaris(const ivar *var, const struct imeta *meta);
    
/* ivar copy */
ivar *ivardup(const ivar *var);

/* make a value */
ivar* ivarmake(const struct imeta* meta, iconstptr value);
    
/* return the value address */
void *ivarvalue(const ivar *var);
  
/* ivar hash code */
uint64_t ivarhashcode(const ivar *var);

/* ivar compare should be the same value-type */
int ivarcompare(const ivar *var, const ivar *nvar);

/* ivar make functions  */
ivar *ivarmakeint(int i);
ivar *ivarmakei64(int64_t i64);
ivar *ivarmakeu64(uint64_t u64);
ivar *ivarmakereal(ireal real);
ivar *ivarmakeptr(iptr ptr);
ivar *ivarmakepod(ipod pod);
ivar *ivarmakeref(irefptr ref);
ivar *ivarmakebool(ibool b);
    
/* ivar destructor */
void ivar_destructor(const struct imeta*, iptr o);

/* ivar meta-funcs: hashcode */
uint64_t ivar_hash(const struct imeta*, iconstptr o);

/* ivar meta-funcs: compare */
int ivar_compare(const struct imeta*, iconstptr lfs, iconstptr rfs);

/* ivar meta-funcs: assign */
void ivar_assign(const struct imeta*, iptr dst, iconstptr src);
    
/* the ivar-cast to type */
#define ivarcast(var, type) (*(type*)(ivarvalue(var)))

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* _IVAR_H_ */
