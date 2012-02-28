//
//  PGCNull.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCNull.h>
#include <PGCFoundation/PGCString.h>

#include <string.h>

#pragma mark Private Function Interfaces

void PGCNullDealloc(PGCType instance);
void PGCNullRelease(PGCType instance);
PGCType PGCNullRetain(PGCType instance);


#pragma mark -

PGCClass *PGCNullClass(void)
{
    static PGCClass *nullClass = NULL;
    if (!nullClass) {
        PGCClassFunctions functions = { PGCNullCopy, PGCNullDealloc, PGCNullDescription, PGCNullEquals, PGCNullHash, PGCNullRelease, PGCNullRetain };
        nullClass = PGCClassCreate("PGCNull", PGCObjectClass(), functions, sizeof(PGCNull));
    }
    return nullClass;
}


PGCNull *PGCNullInstance(void)
{
    static PGCNull *sharedInstance = NULL;
    if (!sharedInstance) {
        sharedInstance = PGCClassAllocateInstance(PGCNullClass());
        PGCObjectInitWithClass(&sharedInstance->super, PGCNullClass());   
    }
    return sharedInstance;
}


#pragma mark Basic Functions

void PGCNullDealloc(PGCType instance)
{    
}


PGCType PGCNullCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCNullClass())) return NULL;
    return instance;
}


PGCString *PGCNullDescription(PGCType instance)
{
    return instance == PGCNullInstance() ? PGCStringNewWithCString("null") : NULL;
}


bool PGCNullEquals(PGCType instance1, PGCType instance2)
{
    return instance1 == instance2 && instance1 == PGCNullInstance();
}


uint64_t PGCNullHash(PGCType instance)
{
    return instance == PGCNullInstance() ? 15486277 : 0;    // A large arbitrary prime number
}


void PGCNullRelease(PGCType instance)
{
}


PGCType PGCNullRetain(PGCType instance)
{
    return instance;
}

