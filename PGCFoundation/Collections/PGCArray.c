//
//  PGCArray.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
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

#include <PGCFoundation/PGCArray.h>

#include <PGCFoundation/PGCString.h>

#include <dispatch/dispatch.h>
#include <string.h>

struct _PGCArray {
    PGCObject super;
    
    PGCObject **objects;
    uint64_t capacity;
    uint64_t increment;
    uint64_t count;
};


#pragma mark Private Global Constants

static const uint64_t PGCArrayDefaultInitialCapacity = 128;


#pragma mark Private Function Interfaces

PGCArray *PGCArrayInitWithObjectAndObjectArguments(PGCArray *array, PGCType firstObject, va_list objectArguments);
void PGCArrayDealloc(PGCType instance);


#pragma mark -

PGCClass *PGCArrayClass(void)
{
    static PGCClass *arrayClass = NULL;
    if (!arrayClass) {
        PGCClassFunctions functions = { PGCArrayCopy, PGCArrayDealloc, PGCArrayDescription, PGCArrayEquals, PGCArrayHash, NULL, NULL };
        arrayClass = PGCClassCreate("PGCArray", PGCObjectClass(), functions, sizeof(PGCArray));
    }
    return arrayClass;
}


PGCArray *PGCArrayInstance(void)
{
    return PGCAutorelease(PGCArrayInit(NULL));
}


PGCArray *PGCArrayInstanceWithObjects(PGCType object1, ...)
{
    va_list arguments;
    
    va_start(arguments, object1);
    PGCArray *array = PGCArrayInitWithObjectAndObjectArguments(NULL, object1, arguments);    
    va_end(arguments);
    
    return PGCAutorelease(array);
}

#pragma mark Basic Functions

PGCArray *PGCArrayInit(PGCArray *array)
{
    return PGCArrayInitWithInitialCapacityAndIncrement(array, 0, 0);
}


PGCArray *PGCArrayInitWithInitialCapacity(PGCArray *array, uint64_t initialCapacity)
{
    return PGCArrayInitWithInitialCapacityAndIncrement(array, initialCapacity, 0);
}


PGCArray *PGCArrayInitWithInitialCapacityAndIncrement(PGCArray *array, uint64_t initialCapacity, uint64_t increment)
{
    if (!array && (array = PGCAlloc(PGCArrayClass())) == NULL) return NULL;
    PGCObjectInit(&array->super);
    
    array->count = 0;
    array->capacity = initialCapacity > 0 ? initialCapacity : PGCArrayDefaultInitialCapacity;
    array->increment = increment;
    array->objects = calloc(array->capacity, sizeof(PGCObject *));
    if (!array->objects) {
        PGCRelease(array);
        return NULL;
    }
    
    return array;
}


PGCArray *PGCArrayInitWithObjects(PGCArray *array, PGCType object1, ...)
{
    va_list arguments;
    
    va_start(arguments, object1);
    array = PGCArrayInitWithObjectAndObjectArguments(array, object1, arguments);    
    va_end(arguments);

    return array;
}


PGCArray *PGCArrayInitWithObjectAndObjectArguments(PGCArray *array, PGCType firstObject, va_list objectArguments)
{
    array = PGCArrayInit(array);
    if (!array) return NULL;

    PGCType object = firstObject;
    while (object) {
        PGCArrayAddObject(array, object);   
        object = va_arg(objectArguments, PGCType);
    }
    
    return array;
}


void PGCArrayDealloc(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCArrayClass())) return;
    PGCArray *array = instance;
    if (array->objects) {
        PGCArrayRemoveAllObjects(array);
        free(array->objects);
    }
    PGCSuperclassDealloc(array);
}


PGCType PGCArrayCopy(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCArrayClass())) return NULL;
    PGCArray *array = instance;
    PGCArray *copy = PGCArrayInitWithInitialCapacityAndIncrement(NULL, array->count, 0);
    for (uint64_t i = 0; i < array->count; i++) PGCArrayAddObject(copy, array->objects[i]);
    return copy;
}


PGCString *PGCArrayDescription(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCArrayClass())) return NULL;
    PGCString *joinString = PGCArrayJoinComponentsWithString(instance, PGCStringInstanceWithCString(", "));
    return PGCStringInstanceWithFormat("[%s]", PGCDescriptionCString(joinString));
}


bool PGCArrayEquals(PGCType instance1, PGCType instance2)
{
    if (!PGCObjectIsKindOfClass(instance1, PGCArrayClass()) || !PGCObjectIsKindOfClass(instance2, PGCArrayClass())) return false;
    
    PGCArray *array1 = instance1;
    PGCArray *array2 = instance2;
    if (array1->count != array2->count) return false;
    
    for (uint64_t i = 0; i < array1->count; i++) {
        PGCType arrayObject1 = array1->objects[i];
        PGCType arrayObject2 = array2->objects[i];
        if (PGCHash(arrayObject1) != PGCHash(arrayObject2) || !PGCEquals(arrayObject1, arrayObject2)) return false;
    }
    
    return true;
}


uint64_t PGCArrayHash(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCArrayClass())) return 0;
    return ((PGCArray *)instance)->count;
}


#pragma mark Accessors

uint64_t PGCArrayGetCount(PGCArray *array)
{
    return array ? array->count : 0;
}


bool PGCArrayContainsObject(PGCArray *array, PGCType instance)
{
    return PGCArrayGetIndexOfObject(array, instance) != PGCNotFound;
}


PGCType PGCArrayGetObjectAtIndex(PGCArray *array, uint64_t index)
{
    return (array && index < array->count) ? PGCAutorelease(PGCRetain(array->objects[index])) : NULL;
}


PGCType PGCArrayGetFirstObject(PGCArray *array)
{
    return PGCArrayGetObjectAtIndex(array, 0);
}


PGCType PGCArrayGetLastObject(PGCArray *array)
{
    return array && array->count > 0 ? PGCArrayGetObjectAtIndex(array, array->count - 1) : NULL;
}


uint64_t PGCArrayGetIndexOfObject(PGCArray *array, PGCType instance)
{
    return array ? PGCArrayGetIndexOfObjectInRange(array, instance, PGCMakeRange(0, array->count)) : PGCNotFound;
}


uint64_t PGCArrayGetIndexOfObjectInRange(PGCArray *array, PGCType instance, PGCRange range)
{
    if (!array || !instance || array->count == 0 || range.location >= array->count || range.location + range.length > array->count) return PGCNotFound;

    uint64_t instanceHash = PGCHash(instance);    
    uint64_t lastIndex = range.location + range.length;
    
    for (uint64_t i = range.location; i < lastIndex; i++) {
        PGCType object = array->objects[i];
        if (PGCHash(object) == instanceHash && PGCEquals(instance, object)) return i;
    }
    
    return PGCNotFound;
}

uint64_t PGCArrayGetIndexOfIdenticalObject(PGCArray *array, PGCType instance)
{
    return array ? PGCArrayGetIndexOfIdenticalObjectInRange(array, instance, PGCMakeRange(0, array->count)) : PGCNotFound;
}


uint64_t PGCArrayGetIndexOfIdenticalObjectInRange(PGCArray *array, PGCType instance, PGCRange range)
{
    if (!array || !instance || array->count == 0 || range.location >= array->count || range.location + range.length > array->count) return PGCNotFound;

    uint64_t lastIndex = range.location + range.length;
    for (uint64_t i = range.location; i < lastIndex; i++) if (array->objects[i] == instance) return i;
    
    return PGCNotFound;
}


#pragma mark Subarrays

PGCArray *PGCArraySubarrayWithRange(PGCArray *array, PGCRange range)
{
    if (!array || range.location >= array->count || range.location + range.length > array->count) return NULL;

    PGCArray *subarray = PGCArrayInitWithInitialCapacity(NULL, range.length);
    uint64_t lastIndex = range.location + range.length;
    for (uint64_t i = range.location; i < lastIndex; i++) {
        PGCArrayAddObject(subarray, array->objects[i]);
    }

    return PGCAutorelease(subarray);
}

#pragma mark Object Addition, Replacement, and Removal

void PGCArrayAddObject(PGCArray *array, PGCType instance)
{
    PGCArrayInsertObjectAtIndex(array, instance, array->count);
}


void PGCArrayInsertObjectAtIndex(PGCArray *array, PGCType instance, uint64_t index)
{
    if (!array || !instance || index > array->count) return;
    
    // Increase the size of our array if necessary
    if (array->count >= array->capacity) {
        uint64_t newCapacity = array->capacity + (array->increment > 0 ? array->increment : array->capacity);
        
        PGCObject **reallocedObjects = realloc(array->objects, newCapacity * sizeof(PGCObject *));
        if (!reallocedObjects) return;
        array->objects = reallocedObjects;
        array->capacity = newCapacity;
    }
    
    // Shift all the objects starting at index up one
    memmove(&array->objects[index + 1], &array->objects[index], (array->count - index) * sizeof(PGCObject *));
    array->objects[index] = PGCRetain(instance);
    array->count++;
}


void PGCArrayPushObject(PGCArray *array, PGCType instance)
{
    PGCArrayInsertObjectAtIndex(array, instance, 0);
}

void PGCArrayExchangeObjectsAtIndices(PGCArray *array, uint64_t index1, uint64_t index2)
{
    if (!array || index1 >= array->count || index2 >= array->count || array->objects[index1] == array->objects[index2]) return;
    PGCType index1Object = array->objects[index1];
    array->objects[index1] = array->objects[index2];
    array->objects[index2] = index1Object;
}


void PGCArrayReplaceObjectAtIndex(PGCArray *array, PGCType instance, uint64_t index)
{
    if (!array || index >= array->count || array->objects[index] == instance) return;
    PGCRelease(array->objects[index]);
    array->objects[index] = PGCRetain(instance);
}


PGCType PGCArrayPopObject(PGCArray *array)
{
    if (!array || array->count == 0) return NULL;
    PGCType poppedInstance = PGCArrayGetObjectAtIndex(array, 0);
    PGCArrayRemoveObjectAtIndex(array, 0);
    return poppedInstance;
}


void PGCArrayRemoveObject(PGCArray *array, PGCType instance)
{
    uint64_t indexOfObject; 
    while ((indexOfObject = PGCArrayGetIndexOfObject(array, instance)) != PGCNotFound) PGCArrayRemoveObjectAtIndex(array, indexOfObject);
}


void PGCArrayRemoveObjectAtIndex(PGCArray *array, uint64_t index)
{
    if (!array || index >= array->count) return;
    PGCRelease(array->objects[index]);
    
    // Shift all the objects starting at index + 1 down one
    memmove(&array->objects[index], &array->objects[index + 1], (array->count - index - 1) * sizeof(PGCObject *));
    --array->count;
}


void PGCArrayRemoveAllObjects(PGCArray *array)
{
    if (!array) return;
    for (uint64_t i = 0; i < array->count; i++) PGCRelease(array->objects[i]); 
    array->count = 0;
}


#pragma mark String Conversion

PGCString *PGCArrayJoinComponentsWithString(PGCArray *array, PGCString *separator)
{
    if (!array) return NULL;
    PGCAutoreleasePool *pool = PGCAutoreleasePoolCreate();
    
    PGCString *join = PGCStringInit(NULL);
    for (uint64_t i = 0; i < array->count; i++) {
        PGCStringAppendString(join, PGCDescription(array->objects[i]));
        if (i != array->count - 1) PGCStringAppendString(join, separator);
    }
    
    PGCAutoreleasePoolDestroy(pool);
    return PGCAutorelease(join);
}


#pragma mark Memory Efficiency

void PGCArrayCondense(PGCArray *array)
{
    if (!array || array->capacity == array->count || (array->count == 0 && array->capacity <= PGCArrayDefaultInitialCapacity)) return;

    uint64_t newCapacity = array->count != 0 ? array->count : PGCArrayDefaultInitialCapacity;
    PGCObject **reallocedObjects = realloc(array->objects, newCapacity * sizeof(PGCObject *));
    if (!reallocedObjects) return;
    
    array->objects = reallocedObjects;
    array->capacity = newCapacity;
}


#pragma mark Enumeration

void PGCArrayEnumerateObjectsWithBlock(PGCArray *array, PGCEnumerationOptions options, PGCIndexedEnumerationBlock block)
{
    if (!array || !block || array->count == 0) return;
    
    bool concurrent = (options & PGCEnumerationConcurrent) != 0;
    bool reverse = (options & PGCEnumerationReverse) != 0;
    
    dispatch_queue_t queue = concurrent ? dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0) : dispatch_queue_create(NULL, NULL);
    __block bool stop = false;
    
    if (!reverse) {
        dispatch_apply(array->count, queue, ^(size_t i) {
            if (stop) return;
            block(array->objects[i], i, &stop);
        });
    } else {
        dispatch_apply(array->count, queue, ^(size_t i) {
            if (stop) return;
            uint64_t index = array->count - i - 1;
            block(array->objects[index], index, &stop);
        });
    }

    if (!concurrent) dispatch_release(queue);
}
