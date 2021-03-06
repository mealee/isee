#ifndef _IARRAYTYPES_H_
#define _IARRAYTYPES_H_
#include "foundation/container/iarray.h"


/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
    
/* forward declares */
struct imeta;
     


/*************************************************************/
/* iarray: int, ireal, int64, char,                          */
/* iarray: iref                                              */
/*************************************************************/

/* the copyable array */
iarray* iarraymakecopyable(size_t capacity, size_t size);
    
/* the copyable array with cmp */
iarray* iarraymakecopyablewith(size_t capacity, size_t size, iarray_entry_cmp cmp);
    
/* the copyable array with meta */
iarray* iarraymakecopyablewithmeta(size_t capacity, const struct imeta *elem);
    
/* tracing the index change in array with ref */
typedef void (*irefarray_index_change) (iptr x, iarray *arr, iref *ref, int index);

/* append to iarray with iref entry */
typedef struct irefarrayentry {
    irefarray_index_change indexchange;
} irefarrayentry;

/* array-iref */
iarray* iarraymakeiref(size_t capacity);

/* array-iref tracing the indexing change */
iarray* iarraymakeirefwithentry(size_t capacity, const irefarrayentry *refentry);
    
/* array-iref with anthor cmp */
iarray* iarraymakeirefwithcmp(size_t capacity, iarray_entry_cmp cmp);

/* array-iref with entry anthor cmp */
iarray* iarraymakeirefwithentryandcmp(size_t capacity, const irefarrayentry *refentry, iarray_entry_cmp cmp);
    
/* array-int */
iarray* iarraymakeint(size_t capacity);

/* array-ireal */
iarray* iarraymakeireal(size_t capacity);

/* array-int64 */
iarray* iarraymakeint64(size_t capacity);
    
/* array-uint64 */
iarray* iarraymakeuint64(size_t capacity);

/* array-char */
iarray* iarraymakechar(size_t capacity);
    
/* array-ibyte */
iarray* iarraymakeibyte(size_t capacity);
    
/* array-ivar */
iarray* iarraymakeivar(size_t capacity);
    
/* the inner array entry type */
typedef enum EnumArrayEntryType {
    EnumArrayEntryType_Char,
    EnumArrayEntryType_Int,
    EnumArrayEntryType_Int64,
    EnumArrayEntryType_UInt64,
    EnumArrayEntryType_Real,
}EnumArrayEntryType;
    
/* get entry */
const iarrayentry* iarrayentryget(int type);
    
/* macro: declare a copy-array-type */
#define __ideclare_array_copy_type(type) iarray* iarraymake##type(size_t capacity);
/* all array types with copy-assign behaviors */
#define __iall_array_types \
    __ideclare_array_copy_type(ipos)\
    __ideclare_array_copy_type(ipos3)\
    __ideclare_array_copy_type(isize)\
    __ideclare_array_copy_type(icircle)\
    __ideclare_array_copy_type(ivec2)\
    __ideclare_array_copy_type(ivec3)\
    __ideclare_array_copy_type(ivec4)\
    __ideclare_array_copy_type(iline2d)\
    __ideclare_array_copy_type(iline3d)\
    __ideclare_array_copy_type(iplane)\
    __ideclare_array_copy_type(imat4)\
    __ideclare_array_copy_type(irect)

/* all copyable-array-types */
__iall_array_types

/* macro: indexing */
#define iarrayof(arr, type, i) (((type *)iarrayat((arr), (i)))[0])
#define iarrayoflast(arr, type) (((type *)iarraylast((arr)))[0])
#define iarrayoffirst(arr, type) (((type *)iarrayfirst((arr)))[0])

/* Helper-Macro: For-Earch in c89 */
#define irangearraycin(arr, type, begin, end, key, value, wrap) \
do { \
    for(key=begin; key<end; ++key) {\
        value = iarrayof(arr, type, key);\
        wrap;\
} } while(0)

/* Helper-Macro: For-Earch in c89 */
#define irangearrayc(arr, type, key, value, wrap) \
irangearraycin(arr, type, 0, iarraylen(arr), key, value, wrap)

/* Helper-Macro: For-Earch in cplusplus */
#define irangearrayin(arr, type, begin, end, wrap) \
do { \
    int __key; \
    type __value; \
    irangearraycin(arr, type, begin, end, __key, __value, wrap); \
} while(0)

/* Helper-Macro: For-Earch in cplusplus */
#define irangearray(arr, type, wrap) \
    irangearrayin(arr, type, 0, iarraylen(arr), wrap)
    
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif


#endif
