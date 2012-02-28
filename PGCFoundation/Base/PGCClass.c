//
//  PGCClass.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/24/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCClass.h>

#include <assert.h>
#include <string.h>

#pragma mark Private Functions

bool PGCClassFunctionsNoneNull(PGCClassFunctions functions)
{
    return functions.copy && functions.dealloc && functions.description && functions.equals && functions.hash && functions.release && functions.retain;
}


#pragma mark Creation and Deallocation

PGCClass *PGCClassCreate(const char *name, PGCClass *superclass, PGCClassFunctions functions, uint64_t instanceSize)
{
    assert(name != NULL);
    
    PGCClass *class = calloc(1, sizeof(PGCClass));
    if (!class) return NULL;
    
    class->name = strdup(name);
    class->superclass = superclass;
    class->functions = functions;
    class->instanceSize = instanceSize;
    
    PGCClass *classIterator = class;
    while (!PGCClassFunctionsNoneNull(class->functions) && (classIterator = classIterator->superclass)) {
        if (!class->functions.copy) class->functions.copy = classIterator->functions.copy;
        if (!class->functions.dealloc) class->functions.dealloc = classIterator->functions.dealloc;
        if (!class->functions.description) class->functions.description = classIterator->functions.description;
        if (!class->functions.equals) class->functions.equals = classIterator->functions.equals;
        if (!class->functions.hash) class->functions.hash = classIterator->functions.hash;
        if (!class->functions.release) class->functions.release = classIterator->functions.release;
        if (!class->functions.retain) class->functions.retain = classIterator->functions.retain;
    }
    
    return class;
}


void PGCClassDestroy(PGCClass *class)
{
    if (!class) return;
    free((void *)class->name);
    class->superclass = NULL;
    free(class);
}


#pragma mark Accessors

const char *PGCClassGetName(PGCClass *class) 
{
    return class ? class->name : NULL;
}


PGCClass *PGCClassGetSuperclass(PGCClass *class)
{
    return class ? class->superclass : NULL;
}


uint64_t PGCClassGetInstanceSize(PGCClass *class)
{
    return class ? class->instanceSize : 0;
}


#pragma mark Instance Allocation

PGCType PGCClassAllocateInstance(PGCClass *class)
{
    return class ? calloc(1, class->instanceSize) : NULL;
}


#pragma mark Class Introspection

bool PGCClassIsSubclass(PGCClass *class, PGCClass *subclass)
{
    if (!class || !subclass) return false;
    
    PGCClass *classIterator = subclass;
    while ((classIterator = classIterator->superclass)) {
        if (classIterator == class) return true;
    }
    
    return false;
}


#pragma mark Class Function Accessors

PGCCopyFunction *PGCClassGetCopyFunction(PGCClass *class)
{
    return class ? class->functions.copy : NULL;
}


PGCDeallocFunction *PGCClassGetDeallocFunction(PGCClass *class)
{
    return class ? class->functions.dealloc : NULL;
}


PGCDescriptionFunction *PGCClassGetDescriptionFunction(PGCClass *class)
{
    return class ? class->functions.description : NULL;
}


PGCEqualsFunction *PGCClassGetEqualsFunction(PGCClass *class) 
{
    return class ? class->functions.equals : NULL;
}


PGCHashFunction *PGCClassGetHashFunction(PGCClass *class)
{
    return class ? class->functions.hash : NULL;
}


PGCReleaseFunction *PGCClassGetReleaseFunction(PGCClass *class)
{
    return class ? class->functions.release : NULL;
}


PGCRetainFunction *PGCClassGetRetainFunction(PGCClass *class)
{
    return class ? class->functions.retain : NULL;
}

