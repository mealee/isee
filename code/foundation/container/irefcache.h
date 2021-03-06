#ifndef _IREFCACHE_H_
#define _IREFCACHE_H_
#include "foundation/core/iref.h"
#include "foundation/container/ireflist.h"


/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
    
/* ref constructor */
typedef iref* (*icachenewentry)();

/* callback: when a ref added to the cache */
typedef void (*icacheaddentry)(iref *ref);

/* callback: when a ref envicted from the cache */
typedef void (*icacheenvictedentry)(iref *ref);

/* Cache */
/* poll and push ref througth the cache */
typedef struct irefcache{
    irefwatcherdeclare;
    
    iname name;
    ireflist* cache;
    size_t capacity;
    uint32_t flag;
    
    icachenewentry newentry;
    icacheenvictedentry envicted;
    icacheaddentry whenadd;
}irefcache;

/* irefcache release all the memory */
void irefcache_destructor(const struct imeta*, iptr o);

/* make a ref cache with capacity */
irefcache *irefcachemake(size_t capacity, icachenewentry newentry);

/* poll one ref from the cache */
iref *irefcachepoll(irefcache *cache);

/* release the ref to cache */
void irefcachepush(irefcache *cache, iref *ref);

/* clear the cache refs */
void irefcacheclear(irefcache *cache);

/* get num of ref in the cache */
size_t irefcachesize(irefcache *cache);

/* macro: get */
#define icache(cache, type) ((type*)irefcachepoll(cache))
/* macro: put */
#define icacheput(cache, ref) irefcachepush(cache, (iref*)(ref))
    
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif


#endif
