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


#pragma mark Basic Functions

PGCObject *PGCObjectInitWithClass(PGCObject *object, PGCClass *class)
{
    assert(class != NULL); 
    
    // If object is NULL attempt to allocate a fresh version
    if (!object && (object = PGCClassAllocateInstance(PGCObjectClass())) == NULL) return NULL;
    
    object->class = class;
    object->retainCount = 1;
    
    return object;
}


void PGCObjectDealloc(PGCType instance)
{
    if (instance) free(instance); 
}


PGCString *PGCObjectDescription(PGCType instance)
{
    if (!instance) return NULL;
    PGCObject *object = instance;
    
    const char *className = PGCClassGetName(object->class);
    
    // Overall length should be class name + hex chars for our pointer (two per byte) + six for the "0x", angle brackets, space, and NULL
    size_t descriptionSize = strlen(className) + sizeof(void *) * 2 + 6; 
    char *description = calloc(descriptionSize, sizeof(char));
    if (!description) return NULL;
    
    // Try to write the description to our buffer. If it doesn't fit, realloc and try again
    int requiredSize = snprintf(description, descriptionSize, "<%s %p>", className, object) + 1;
    if (requiredSize > descriptionSize) {
        descriptionSize = requiredSize;
        
        // If we could successfully reallocate, try writing again. Otherwise, we give up
        char *newDescription = realloc(description, sizeof(char) * descriptionSize);
        if (newDescription) {
            description = newDescription;
            snprintf(description, descriptionSize, "<%s %p>", className, object);   
        }
    }
    
    PGCString *string = PGCStringNewWithCString(description);
    free(description);
    return string;
}


bool PGCObjectEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1 || !instance2) return false;
    return instance1 == instance2;
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
    return instance ? ((PGCObject *)instance)->class : NULL;
}


bool PGCObjectIsKindOfClass(PGCType instance, PGCClass *class)
{
    if (!instance || !class) return false;
    PGCObject *object = instance;
    return object->class == class || PGCClassIsSubclass(class, object->class);
}


#pragma mark Polymorphic Functions

PGCType PGCCopy(PGCType instance)
{
    PGCCopyFunction *copy = PGCObjectGetCopyFunction(instance);
    return copy ? copy(instance) : NULL;
}


void PGCDealloc(PGCType instance)
{
    PGCDeallocFunction *dealloc = PGCObjectGetDeallocFunction(instance);
    if (dealloc) dealloc(instance);
}


PGCString *PGCDescription(PGCType instance)
{
    PGCDescriptionFunction *description = PGCObjectGetDescriptionFunction(instance);
    return description ? description(instance) : NULL;
}


const char *PGCDescriptionCString(PGCType instance)
{
    return PGCStringGetCString(PGCDescription(instance));
}


bool PGCEquals(PGCType instance1, PGCType instance2)
{
    PGCEqualsFunction *equals = PGCObjectGetEqualsFunction(instance1);
    return equals ? equals(instance1, instance2) : false;
}


uint64_t PGCHash(PGCType instance)
{
    PGCHashFunction *hash = PGCObjectGetHashFunction(instance);
    return hash ? hash(instance) : 0;
}


PGCType PGCRetain(PGCType instance)
{
    PGCRetainFunction *retain = PGCObjectGetRetainFunction(instance);
    return retain ? retain(instance) : NULL;
}


void PGCRelease(PGCType instance)
{
    PGCReleaseFunction *release = PGCObjectGetReleaseFunction(instance);
    if (release) release(instance);
}


PGCType PGCAutorelease(PGCType instance)
{
    PGCAutoreleasePoolAddObject(instance);
    return instance;
}


PGCType PGCSuperclassCopy(PGCType instance)
{
    if (!instance) return NULL;
    PGCCopyFunction *copy = PGCClassGetCopyFunction(((PGCObject *)instance)->class->superclass);
    return copy ? copy(instance) : NULL;
}


void PGCSuperclassDealloc(PGCType instance)
{
    if (!instance) return;
    PGCDeallocFunction *dealloc = PGCClassGetDeallocFunction(((PGCObject *)instance)->class->superclass);
    if (dealloc) dealloc(instance);
}


PGCString *PGCSuperclassDescription(PGCType instance)
{
    if (!instance) return NULL;
    PGCDescriptionFunction *description = PGCClassGetDescriptionFunction(((PGCObject *)instance)->class->superclass);
    return description ? description(instance) : NULL;
}


bool PGCSuperclassEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1) return false;
    PGCEqualsFunction *equals = PGCClassGetEqualsFunction(((PGCObject *)instance1)->class->superclass);
    return equals ? equals(instance1, instance2) : false;
}


uint64_t PGCSuperclassHash(PGCType instance)
{
    if (!instance) return 0;
    PGCHashFunction *hash = PGCClassGetHashFunction(((PGCObject *)instance)->class->superclass);
    return hash ? hash(instance) : 0;
}


PGCType PGCSuperclassRetain(PGCType instance)
{
    if (!instance) return NULL;
    PGCRetainFunction *retain = PGCClassGetRetainFunction(((PGCObject *)instance)->class->superclass);
    return retain ? retain(instance) : NULL;
}


void PGCSuperclassRelease(PGCType instance)
{
    if (!instance) return;
    PGCReleaseFunction *release = PGCClassGetReleaseFunction(((PGCObject *)instance)->class->superclass);
    if (release) release(instance);
}


#pragma mark Convenience Accessors for Class Functions

PGCCopyFunction *PGCObjectGetCopyFunction(PGCType instance)
{
    return instance ? PGCClassGetCopyFunction(((PGCObject *)instance)->class) : NULL;
}


PGCDeallocFunction *PGCObjectGetDeallocFunction(PGCType instance)
{
    return instance ? PGCClassGetDeallocFunction(((PGCObject *)instance)->class) : NULL;
}


PGCDescriptionFunction *PGCObjectGetDescriptionFunction(PGCType instance)
{
    return instance ? PGCClassGetDescriptionFunction(((PGCObject *)instance)->class) : NULL;
}


PGCEqualsFunction *PGCObjectGetEqualsFunction(PGCType instance) 
{
    return instance ? PGCClassGetEqualsFunction(((PGCObject *)instance)->class) : NULL;
}


PGCHashFunction *PGCObjectGetHashFunction(PGCType instance)
{
    return instance ? PGCClassGetHashFunction(((PGCObject *)instance)->class) : NULL;
}


PGCReleaseFunction *PGCObjectGetReleaseFunction(PGCType instance)
{
    return instance ? PGCClassGetReleaseFunction(((PGCObject *)instance)->class) : NULL;
}


PGCRetainFunction *PGCObjectGetRetainFunction(PGCType instance)
{
    return instance ? PGCClassGetRetainFunction(((PGCObject *)instance)->class) : NULL;
}


