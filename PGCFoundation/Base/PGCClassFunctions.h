//
//  PGCClassFunctions.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/24/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCFoundation_PGCClassFunctions_h
#define PGCFoundation_PGCClassFunctions_h

#include <PGCFoundation/PGCBase.h>

#pragma mark - PGCClassFunctions

typedef PGCType PGCCopyFunction(PGCType instance);
typedef void PGCDeallocFunction(PGCType instance);
typedef char *PGCDescriptionFunction(PGCType instance);
typedef bool PGCEqualsFunction(PGCType instance1, PGCType instance2);
typedef uint64_t PGCHashFunction(PGCType instance);
typedef void PGCReleaseFunction(PGCType instance);
typedef PGCType PGCRetainFunction(PGCType instance);

typedef struct _PGCClassFunctions {
    PGCCopyFunction *copy;
    PGCDeallocFunction *dealloc;
    PGCDescriptionFunction *description;
    PGCEqualsFunction *equals;
    PGCHashFunction *hash;
    PGCReleaseFunction *release;
    PGCRetainFunction *retain;
} PGCClassFunctions;


#pragma mark Creation and Deallocation

extern PGCClassFunctions *PGCClassFunctionsCreate(PGCCopyFunction *copy, PGCDeallocFunction *dealloc, PGCDescriptionFunction *description, 
                                                  PGCEqualsFunction *equals, PGCHashFunction *hash, PGCReleaseFunction *release, 
                                                  PGCRetainFunction *retain);
extern void PGCClassFunctionsDealloc(PGCClassFunctions *classFunctions);


#pragma mark Convenience Tests

extern bool PGCClassFunctionsNoneNull(PGCClassFunctions *functions);

#endif
