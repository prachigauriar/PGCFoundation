//
//  PGCBase.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/24/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCBASE_H
#define PGCBASE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef void *PGCType;

typedef struct _PGCString PGCString;

typedef enum _PGCComparisonResult PGCComparisonResult;

enum _PGCComparisonResult {
    PGCOrderedDescending = -1,
    PGCOrderedSame,
    PGCOrderedAscending
};

typedef PGCType PGCCopyFunction(PGCType instance);
typedef void PGCDeallocFunction(PGCType instance);
typedef PGCString *PGCDescriptionFunction(PGCType instance);
typedef bool PGCEqualsFunction(PGCType instance1, PGCType instance2);
typedef uint64_t PGCHashFunction(PGCType instance);
typedef void PGCReleaseFunction(PGCType instance);
typedef PGCType PGCRetainFunction(PGCType instance);

typedef struct _PGCClassFunctions PGCClassFunctions;

struct _PGCClassFunctions {
    PGCCopyFunction *copy;
    PGCDeallocFunction *dealloc;
    PGCDescriptionFunction *description;
    PGCEqualsFunction *equals;
    PGCHashFunction *hash;
    PGCReleaseFunction *release;
    PGCRetainFunction *retain;
};

extern const uint64_t PGCNotFound;

#endif
