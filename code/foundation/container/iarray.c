#include "foundation/container/iarray.h"
#include "foundation/memory/imemory.h"
#include "foundation/core/imetatypes.h"
#include "foundation/container/iheap.h"

/*invalid index */
const int kindex_invalid = -1;

/* 释放数组相关的资源 */
void iarray_destructor(ithis x, iobj *o) {
    iarray *array = icast(iarray, __iref(o));
    
    /* unset the autoshirnk flag */
    iarrayunsetflag(array, EnumArrayFlagAutoShirk);
    /* unset the sliced flag */
    iarrayunsetflag(array, EnumArrayFlagSliced);
    
    /* release all element */
    iarraytruncate(array, 0);
    /* release the raw buffer */
    ifree(array->buffer);
    /*
     array->buffer = NULL;
     array->len = 0;
     array->capacity = 0;
     array->entry = NULL;
     array->free = NULL;
     array->flag = 0;
     */
}

/* make array */
iarray *iarraymake(size_t capacity, const iarrayentry *entry) {
    iarray *array = (iarray *)iobjmalloc(iarray);
    array->capacity = capacity;
    array->len = 0;
    array->buffer = capacity > 0 ? (char*)icalloc(capacity, entry->size) : NULL;
    array->entry = entry;
    array->flag = entry->flag;
    array->cmp = entry->cmp;
    iretain(array);
    
    return array;
}

/* length */
size_t iarraylen(const iarray *arr) {
    icheckret(arr, 0);
    return arr->len;
}

/* capacity*/
size_t iarraycapacity(const iarray *arr) {
    icheckret(arr, 0);
    return arr->capacity;
}

/* indexing */
const void* iarrayat(const iarray *arr, int index) {
    icheckret(arr, NULL);
    icheckret(index>=0 && index<arr->len, NULL);
    
    return  __arr_i(arr, index);
}

/* the raw buffer */
void* iarraybuffer(iarray *arr) {
    return arr->buffer;
}

/* set flag */
int iarraysetflag(iarray *arr, int flag) {
    int old = arr->flag;
    arr->flag |= flag;
    return old;
}

/* clear flag */
int iarrayunsetflag(iarray *arr, int flag){
    int old = arr->flag;
    arr->flag &= (~flag);
    return old;
}

/* have flag ?? */
int iarrayisflag(const iarray *arr, int flag) {
    return arr->flag & flag;
}

/* operators: remove */
int iarrayremove(iarray *arr, int index) {
    int i;
    
    icheckret(arr, iino);
    icheckret(index>=0 && index<arr->len, iino);
    /* sliced array can not be removed */
    icheckret(!iarrayisflag(arr, EnumArrayFlagSliced), iino);
    
    if (!(arr->entry->flag & EnumArrayFlagSimple)) {
        arr->entry->swap(arr, index, kindex_invalid);
    }
    
    if (arr->flag & EnumArrayFlagKeepOrder) {
        /* move element before */
        for(i=index; i<arr->len-1; ++i) {
            arr->entry->swap(arr, i, i+1);
        }
    } else if (arr->len-1 != index){
        /* directly swap the last element */
        arr->entry->swap(arr, index, arr->len-1);
    }
    --arr->len;
    return iiok;
}

/* NB!! should besure the redundant element have been destruct before call this */
static size_t _iarray_just_capacity(iarray *arr, size_t newcapacity) {
    char* newbuffer;
    newbuffer = irealloc(arr->buffer, newcapacity * arr->entry->size);
    icheckret(newbuffer, arr->capacity);
    /* 清理新加的内存 */
    if (arr->flag & EnumArrayFlagMemsetZero && newcapacity > arr->capacity) {
        memset(newbuffer + arr->capacity * arr->entry->size,
               0,
               (newcapacity-arr->capacity) * arr->entry->size);
    }
    
    arr->buffer = newbuffer;
    arr->capacity = newcapacity;
    return arr->capacity;
}

/* besure arr->capacity >= capacity, return the new capacity */
static size_t _iarray_be_capacity(iarray *arr, size_t capacity) {
    size_t newcapacity;
    
    icheckret(arr->capacity < capacity, arr->capacity);
    
    /* new capacity */
    newcapacity = arr->capacity;
    do {
        newcapacity = newcapacity * 2;
    } while(newcapacity < capacity);
    
    return _iarray_just_capacity(arr, newcapacity);
}

/* auto shrink the array capacity */
static void _iarrayautoshrink(iarray *arr) {
    size_t suppose = imax(arr->len * 2, 8);
    if (arr->capacity > suppose) {
        iarrayshrinkcapacity(arr, suppose);
    }
}

/* operators: add */
int iarrayadd(iarray *arr, const void* value) {
    return iarrayinsert(arr, arr->len, value, 1);
}

/* operators: insert */
int iarrayinsert(iarray *arr, int index, const void *value, int nums) {
    int i;
    
    /* check if we need do insert */
    icheckret(nums > 0, iiok);
    /* check if the index belong to [0, arr->len] */
    icheckret(index>=0 && index<=arr->len, iino);
    /* the sliced array can not extend capacity */
    icheckret(!iarrayisflag(arr, EnumArrayFlagSliced)
              || (arr->len + nums) <= arr->capacity, iino);
    /* be sure the capacity is enough */
    _iarray_be_capacity(arr, arr->len + nums);
    /* check if we have been got enough space to do inserting*/
    icheckret(arr->capacity >= arr->len + nums, iino);
    
    /*swap after*/
    if (index != arr->len) {
        /* if the array is simple one, we can just do memove */
        /* simple flag is only for inner use */
        if (arr->entry->flag & EnumArrayFlagSimple) {
            arr->entry->assign(arr, index + nums,
                               __arr_i(arr, index),
                               arr->len - index);
        } else {
            /* swap one by one */
            i = arr->len - 1;
            while (i >= index) {
                arr->entry->swap(arr, i, i+nums);
                --i;
            }
        }
    }
    /* assign it */
    arr->entry->assign(arr, index, value, nums);
    arr->len += nums;
    return iiok;
}

/* operators: set value at index */
int iarrayset(iarray *arr, int index, const void *value) {
    icheckret(index >=0 && index<arr->len, iino);
    arr->entry->assign(arr, index, value, 1);
    return iiok;
}

/* remove all elements */
void iarrayremoveall(iarray *arr) {
    iarraytruncate(arr, 0);
}

/* truncating the array to len */
void iarraytruncate(iarray *arr, size_t len) {
    int i;
    
    icheck(arr);
    icheck(arr->len > len);
    /* sliced array can not be truncate */
    icheck(!iarrayisflag(arr, EnumArrayFlagSliced));
    
    if (arr->entry->flag & EnumArrayFlagSimple) {
        /* direct set the length*/
        arr->len = len;
        /* auto shirk */
        if (arr->flag & EnumArrayFlagAutoShirk) {
            _iarrayautoshrink(arr);
        }
    } else {
        /* remove one by one*/
        for(i=arr->len; i>len; i--) {
            iarrayremove(arr, i-1);
        }
    }
}

/* shrink the capacity  */
size_t iarrayshrinkcapacity(iarray *arr, size_t capacity) {
    icheckret(arr->capacity > capacity, arr->capacity);
    
    /* sliced array can not be shrink */
    icheckret(!iarrayisflag(arr, EnumArrayFlagSliced), arr->capacity);
    
    capacity = imax(arr->len, capacity);
    return _iarray_just_capacity(arr, capacity);
}

/* expand the capacity */
size_t iarrayexpandcapacity(iarray *arr, size_t capacity) {
    icheckret(arr->capacity < capacity, arr->capacity);
    
    return _iarray_just_capacity(arr, capacity);
}

/* sort */
void iarraysort(iarray *arr) {
    icheck(arr->len);
    
    /* algorithm: heap-sort */
    iarraysortheap(arr, 0, arr->len-1);
}

/* for each */
void iarrayforeach(const iarray *arr, iarray_entry_visitor visitor) {
    size_t size = iarraylen(arr);
    size_t idx = 0;
    for (; idx < size; ++idx) {
        visitor(arr, idx, iarrayat(arr, idx));
    }
}