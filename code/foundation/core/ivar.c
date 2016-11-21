#include "foundation/core/ivar.h"
#include "foundation/core/imeta.h"
#include "foundation/core/imetatypes.h"
#include "foundation/core/iobj.h"
#include "foundation/memory/imemory.h"

    
/* ivar destructor */
void ivar_destructor(iptr x, iptr o) {
    ivar *var = icast(ivar, o);
    if (ivarissimple(var)) {
        /* nothing todo: pod */
    } else if (var->meta == imetaof(ipod)) {
        if (var->v.pod.ptr && var->v.pod.ptr != var->v.pod.stbuf) {
            ifree(var->v.pod.ptr);
            var->v.pod.ptr = var->v.pod.stbuf;
            var->v.pod.size = 0;
            var->v.pod.align = 0;
        }
    } else {
        /* the meta ref */
        irefdelete(var->v.ref);
        var->meta = NULL;
    }
}

/* ivar meta-funcs: hashcode */
uint64_t ivar_hash(iptr x, iptr o) {
    ivar *var = icast(ivar, o);
    return ivarhashcode(var);
}

/* ivar meta-funcs: compare 
 * todos: may be problems in compare with 64 bit
 */
int ivar_compare(iptr x, iptr lfs, iptr rfs) {
    ivar *l = icast(ivar, lfs);
    ivar *r = icast(ivar, rfs);
    const struct imeta* lmeta = iobjgetmeta(l);
    const struct imeta* rmeta = iobjgetmeta(r);
    const struct imeta* meta = lmeta ? lmeta : rmeta;
    if (meta == imetaof(int)) {
        return l->v.i - r->v.i;
    } else if (meta == imetaof(int64_t)) {
        return l->v.i64 - r->v.i64;
    } else if (meta == imetaof(uint64_t)) {
        return l->v.u64 - r->v.u64;
    } else if (meta == imetaof(ireal)) {
        return l->v.real - r->v.real > 0;
    } else if (meta == imetaof(float)) {
        return l->v.real - r->v.real > 0;
    } else if (meta == imetaof(double)) {
        return l->v.real - r->v.real > 0;
    } else if (meta == imetaof(ibyte)) {
        return l->v.i64 - r->v.i64;
    } else if (meta == imetaof(ibool)) {
        return l->v.i64 - r->v.i64;
    } else if (meta == imetaof(irune)) {
        return l->v.i64 - r->v.i64;
    } else if (meta == imetaof(iptr)) {
        return l->v.i64 - r->v.i64;
    } else if (meta == imetaof(inull)) {
        return l->v.i64 - r->v.i64;
    }
    if (meta->funcs && meta->funcs->compare) {
        return meta->funcs->compare(meta->funcs,
                                    l->v.ref,
                                    r->v.ref);
    }
    return (int)((char*)lfs - (char*)rfs);
}
 
/* ivar type */
int ivartype(const ivar *var) {
    icheckret(var && var->meta, 0);
    return var->meta->index;
}

/* ivar type: meta */
const struct imeta *ivarmeta(const ivar *var) {
    icheckret(var, imetaof(inull));
    icheckret(var->meta, imetaof(inull));
    return var->meta;
}

/* simple ivar */
int ivarissimple(const ivar *var) {
    const struct imeta *meta = ivarmeta(var);
    if (meta == imetaof(int) ||
        meta == imetaof(int64_t) ||
        meta == imetaof(uint64_t) ||
        meta == imetaof(ireal) ||
        meta == imetaof(float) ||
        meta == imetaof(double) ||
        meta == imetaof(ibyte) ||
        meta == imetaof(ibool) ||
        meta == imetaof(irune) ||
        meta == imetaof(iptr) ||
        meta == imetaof(inull)) {
        return iiok;
    }
    return iino;
}

/* ivar is right-meta-type */
ibool ivaris(const ivar *var, const struct imeta *meta) {
    return ivartype(var) == imetaindexof(meta);
}
    
/* ivar copy */
ivar *ivardup(const ivar *var) {
    ivar *nvar = nvar = irefnew(ivar);
    if (ivarissimple(var)) {
        nvar->meta = var->meta;
        nvar->v = var->v;
    } else if (var->meta == imetaof(ipod)) {
        nvar->v = var->v;
        nvar->v.pod.ptr = icalloc(1, var->v.pod.size);
        memcpy(nvar->v.pod.ptr, var->v.pod.ptr, var->v.pod.size);
    } else {
        irefretain(var->v.ref);
        nvar->meta = var->meta;
        nvar->v = var->v;
    }
    return nvar;
}
  
/* ivar hash code */
uint64_t ivarhashcode(const ivar *var) {
    if (ivarissimple(var)) {
        return var->v.u64;
    } else if (ivaris(var, imetaof(ipod))) {
        /* todo hash in pod: md5 */
        return (uint64_t)(var->v.pod.ptr);
    } else {
        /* have a hash funcs */
        if (var->meta && var->meta->funcs && var->meta->funcs->hash) {
            return (uint64_t)(var->meta->funcs->hash(var->meta->funcs, __iobj(var->v.ref)));
        }
        /* todo hash in ref-method */
        return (uint64_t)(var->v.ref);
    }
}

/* ivar make functions: int  */
ivar *ivarmakeint(int i) {
    ivar *var = irefnew(ivar);
    var->v.i = i;
    var->meta = imetaof(int);
    return var;
}
/* ivar make functions: int64_t  */
ivar *ivarmakei64(int64_t i64) {
    ivar *var = irefnew(ivar);
    var->v.i64 = i64;
    var->meta = imetaof(int64_t);
    return var;
}
/* ivar make functions: int  */
ivar *ivarmakeu64(uint64_t u64) {
    ivar *var = irefnew(ivar);
    var->v.u64 = u64;
    var->meta = imetaof(uint64_t);
    return var;
}

/* ivar make functions: int  */
ivar *ivarmakereal(ireal real) {
    ivar *var = irefnew(ivar);
    var->v.real = real;
    var->meta = imetaof(ireal);
    return var;
}

/* ivar make functions: int  */
ivar *ivarmakeptr(iptr *ptr) {
    ivar *var = irefnew(ivar);
    var->v.ptr = ptr;
    var->meta = imetaof(iptr);
    return var;
}

/* ivar make functions: int  */
ivar *ivarmakepod(ipod pod) {
    ivar *var = irefnew(ivar);
    var->v.pod = pod;
    var->meta = imetaof(ipod);
    /* the expanded pod */
    if (pod.ptr && pod.ptr != pod.stbuf) {
        var->v.pod.ptr = icalloc(1, pod.size);
        memcpy(var->v.pod.ptr, pod.ptr, pod.size);
    }
    return var;
}

/* ivar make functions: int  */
ivar *ivarmakeref(iref *ref) {
    ivar *var = irefnew(ivar);
    iassign(var->v.ref, ref);
    var->meta = iobjgetmeta(ref);
    return var;
}
 
