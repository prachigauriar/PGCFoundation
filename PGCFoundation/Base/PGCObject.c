//
//  PGCObject.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/22/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCObject.h>
#include <PGCFoundation/PGCAutoreleasePool.h>
#include <PGCFoundation/PGCString.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#pragma mark Private Function Interfaces

void PGCObjectDealloc(PGCType instance);


#pragma mark -

PGCClass *PGCObjectClass()
{
    static PGCClass *objectClass = NULL;
    if (!objectClass) {
        PGCClassFunctions functions = { NULL, PGCObjectDealloc, PGCObjectDescription, PGCObjectEquals, PGCObjectHash, PGCObjectRelease, PGCObjectRetain };
        objectClass = PGCClassCreate("PGCObject", NULL, functions, sizeof(PGCObject));
    }
    return objectClass;
}


PGCType PGCAlloc(PGCClass *class)
{
    if (!PGCClassIsSubclassOfClass(class, PGCObjectClass())) return NULL;
    PGCObject *object = PGCClassAllocateInstance(class);
    if (object) {
        object->isa = class;
        object->retainCount = 1; 
    }
    return object;
}


#pragma mark Basic Functions

PGCObject *PGCObjectInit(PGCObject *object)
{
    return object;
}


void PGCObjectDealloc(PGCType instance)
{
    if (instance) free(instance); 
}


PGCString *PGCObjectDescription(PGCType instance)
{
    return instance ? PGCStringInstanceWithFormat("<%s %p>", PGCClassGetName(((PGCObject *)instance)->isa), instance) : NULL;    
}


bool PGCObjectEquals(PGCType instance1, PGCType instance2)
{
    return instance1 == instance2 && instance1;
}


uint64_t PGCObjectHash(PGCType instance)
{
    return (uint64_t)instance;
}


void PGCObjectRelease(PGCType instance)
{
    PGCObject *object = instance;
    
    // If we have an invalid object or its retain count is 0, we're done
    if (!object || object->retainCount == 0) return;

    // Otherwise, if its retain count is 0 after decrementing, dealloc it
    if (--object->retainCount == 0) PGCDealloc(object);
}


PGCType PGCObjectRetain(PGCType instance)
{
    if (instance) ((PGCObject *)instance)->retainCount++;
    return instance;
}


#pragma mark Class Introspection

PGCClass *PGCObjectGetClass(PGCType instance)
{
    return instance ? ((PGCObject *)instance)->isa : NULL;
}


bool PGCObjectIsKindOfClass(PGCType instance, PGCClass *class)
{
    if (!instance || !class) return false;
    PGCObject *object = instance;
    return object->isa == class || PGCClassIsSubclassOfClass(object->isa, class);
}
