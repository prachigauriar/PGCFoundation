//
//  PGCObject.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/22/2012.
//  Copyright (c) 2012 Prachi Gauriar.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
