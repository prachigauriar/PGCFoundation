//
//  PGCClass.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/24/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCFoundation_PGCClass_h
#define PGCFoundation_PGCClass_h

#include <PGCFoundation/PGCBase.h>

#pragma mark - PGCClass 

typedef struct _PGCClass PGCClass;

struct _PGCClass {
    const char *name;
    struct _PGCClass *superclass;
    PGCClassFunctions functions;
    uint64_t instanceSize;
};

#pragma mark Creation and Deallocation

extern PGCClass *PGCClassCreate(const char *name, PGCClass *superclass, PGCClassFunctions functions, uint64_t instanceSize);
extern void PGCClassDestroy(PGCClass *class);

#pragma mark Accessors

extern const char *PGCClassGetName(PGCClass *class);
extern PGCClass *PGCClassGetSuperclass(PGCClass *class);
extern uint64_t PGCClassGetInstanceSize(PGCClass *class);
extern PGCClassFunctions PGCClassGetClassFunctions(PGCClass *class);

#pragma mark Instance Allocation

extern PGCType PGCClassAllocateInstance(PGCClass *class);

#pragma mark Class Introspection

extern bool PGCClassIsSubclass(PGCClass *class, PGCClass *subclass);

#pragma mark Class Function Accessors

extern PGCCopyFunction *PGCClassGetCopyFunction(PGCClass *class);
extern PGCDeallocFunction *PGCClassGetDeallocFunction(PGCClass *class);
extern PGCDescriptionFunction *PGCClassGetDescriptionFunction(PGCClass *class);
extern PGCEqualsFunction *PGCClassGetEqualsFunction(PGCClass *class);
extern PGCHashFunction *PGCClassGetHashFunction(PGCClass *class);
extern PGCReleaseFunction *PGCClassGetReleaseFunction(PGCClass *class);
extern PGCRetainFunction *PGCClassGetRetainFunction(PGCClass *class);

#endif
