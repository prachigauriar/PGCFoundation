//
//  PGCBase.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCBase.h>
#include <PGCFoundation/PGCAutoreleasePool.h>
#include <PGCFoundation/PGCClass.h>
#include <PGCFoundation/PGCObject.h>
#include <PGCFoundation/PGCString.h>

#pragma mark Constants

const uint64_t PGCNotFound = UINT64_MAX;


#pragma mark Polymorphic Functions

PGCType PGCCopy(PGCType instance)
{
    PGCCopyFunction *copy = PGCClassGetCopyFunction(PGCObjectGetClass(instance));
    return copy ? copy(instance) : NULL;
}


void PGCDealloc(PGCType instance)
{
    PGCDeallocFunction *dealloc = PGCClassGetDeallocFunction(PGCObjectGetClass(instance));
    if (dealloc) dealloc(instance);
}


PGCString *PGCDescription(PGCType instance)
{
    PGCDescriptionFunction *description = PGCClassGetDescriptionFunction(PGCObjectGetClass(instance));
    return description ? description(instance) : NULL;
}


const char *PGCDescriptionCString(PGCType instance)
{
    return PGCStringGetCString(PGCDescription(instance));
}


bool PGCEquals(PGCType instance1, PGCType instance2)
{
    PGCEqualsFunction *equals = PGCClassGetEqualsFunction(PGCObjectGetClass(instance1));
    return equals ? equals(instance1, instance2) : false;
}


uint64_t PGCHash(PGCType instance)
{
    PGCHashFunction *hash = PGCClassGetHashFunction(PGCObjectGetClass(instance));
    return hash ? hash(instance) : 0;
}


PGCType PGCRetain(PGCType instance)
{
    PGCRetainFunction *retain = PGCClassGetRetainFunction(PGCObjectGetClass(instance));
    return retain ? retain(instance) : NULL;
}


void PGCRelease(PGCType instance)
{
    PGCReleaseFunction *release = PGCClassGetReleaseFunction(PGCObjectGetClass(instance));
    if (release) release(instance);
}


PGCType PGCAutorelease(PGCType instance)
{
    PGCAutoreleasePoolAddObject(instance);
    return instance;
}


#pragma mark Polymorphic Superclass Functions

PGCType PGCSuperclassCopy(PGCType instance)
{
    if (!instance) return NULL;
    PGCCopyFunction *copy = PGCClassGetCopyFunction(PGCClassGetSuperclass(PGCObjectGetClass(instance)));
    return copy ? copy(instance) : NULL;
}


void PGCSuperclassDealloc(PGCType instance)
{
    if (!instance) return;
    PGCDeallocFunction *dealloc = PGCClassGetDeallocFunction(PGCClassGetSuperclass(PGCObjectGetClass(instance)));
    if (dealloc) dealloc(instance);
}


PGCString *PGCSuperclassDescription(PGCType instance)
{
    if (!instance) return NULL;
    PGCDescriptionFunction *description = PGCClassGetDescriptionFunction(PGCClassGetSuperclass(PGCObjectGetClass(instance)));
    return description ? description(instance) : NULL;
}


bool PGCSuperclassEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1) return false;
    PGCEqualsFunction *equals = PGCClassGetEqualsFunction(PGCClassGetSuperclass(PGCObjectGetClass(instance1)));
    return equals ? equals(instance1, instance2) : false;
}


uint64_t PGCSuperclassHash(PGCType instance)
{
    if (!instance) return 0;
    PGCHashFunction *hash = PGCClassGetHashFunction(PGCClassGetSuperclass(PGCObjectGetClass(instance)));
    return hash ? hash(instance) : 0;
}


PGCType PGCSuperclassRetain(PGCType instance)
{
    if (!instance) return NULL;
    PGCRetainFunction *retain = PGCClassGetRetainFunction(PGCClassGetSuperclass(PGCObjectGetClass(instance)));
    return retain ? retain(instance) : NULL;
}


void PGCSuperclassRelease(PGCType instance)
{
    if (!instance) return;
    PGCReleaseFunction *release = PGCClassGetReleaseFunction(PGCClassGetSuperclass(PGCObjectGetClass(instance)));
    if (release) release(instance);
}
