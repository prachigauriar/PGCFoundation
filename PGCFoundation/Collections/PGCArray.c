//
//  PGCArray.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCArray.h>
#include <PGCFoundation/PGCString.h>

#include <string.h>

#pragma mark Private Global Constants

static const uint64_t PGCArrayDefaultInitialCapacity = 128;

#pragma mark Private Function Interfaces

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


void PGCArrayDealloc(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCArrayClass())) return;
    PGCArray *array = instance;
    if (array->objects) {
        PGCArrayRemoveAllObjects(array);
        free(array->objects);
    }
    PGCSuperclassDealloc(array);
}


PGCType PGCArrayCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCArrayClass())) return NULL;
    PGCArray *array = instance;
    
    PGCArray *copy = PGCArrayInitWithInitialCapacityAndIncrement(NULL, array->count, 0);
    for (uint64_t i = 0; i < array->count; i++) PGCArrayAddObject(copy, PGCArrayGetObjectAtIndex(array, i));
    
    return copy;
}


PGCString *PGCArrayDescription(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCArrayClass())) return NULL;
    PGCString *joinString = PGCArrayJoinComponentsWithString(instance, PGCStringInstanceWithCString(", "));
    return PGCStringInstanceWithFormat("[%s]", PGCDescriptionCString(joinString));
}


bool PGCArrayEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1 || !instance2) return false;
    if (!PGCObjectIsKindOfClass(instance1, PGCArrayClass()) || !PGCObjectIsKindOfClass(instance2, PGCArrayClass())) return false;

    PGCArray *array1 = instance1;
    PGCArray *array2 = instance2;
    if (array1->count != array2->count) return false;
        
    for (uint64_t i = 0; i < array1->count; i++) {
        PGCType arrayObject1 = PGCArrayGetObjectAtIndex(array1, i);
        PGCType arrayObject2 = PGCArrayGetObjectAtIndex(array2, i);
        
        if (PGCHash(arrayObject1) != PGCHash(arrayObject2)) return false;
        if (!PGCEquals(arrayObject1, arrayObject2)) return false;
    }
    
    return true;
}


uint64_t PGCArrayHash(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCArrayClass())) return 0;
    PGCArray *array = instance;
    
    // This is the DJB2 hash function, which is a fast, public domain string hash with
    // excellent distribution. It was posted to comp.lang.c by Daniel J. Bernstein.
    // It has been adapted slightly to deal with an array of objects instead of an 
    // array of characters
    uint64_t hash = 5381 + array->count;
    for (uint64_t i = 0; i < array->count; i++) {
        // Set hash to hash * 33 + the current element's value
        hash = ((hash << 5) + hash) + PGCHash(PGCArrayGetObjectAtIndex(array, i));
    }
    
    return hash;
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
    if (!array || !instance || array->count == 0) return PGCNotFound;

    uint64_t instanceHash = PGCHash(instance);
    for (uint64_t i = 0; i < array->count; i++) {
        PGCType object = PGCArrayGetObjectAtIndex(array, i);
        if (PGCHash(object) == instanceHash && PGCEquals(instance, object)) return i;
    }
    
    return PGCNotFound;
}


uint64_t PGCArrayGetIndexOfIdenticalObject(PGCArray *array, PGCType instance)
{
    if (!array || !instance || array->count == 0) return PGCNotFound;
    for (uint64_t i = 0; i < array->count; i++) if (PGCArrayGetObjectAtIndex(array, i) == instance) return i;
    return PGCNotFound;
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
    if (!array || index >= array->count) return;
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
    memmove(&array->objects[index], &array->objects[index + 1], (array->count - index - 1) * sizeof(PGCObject *));
    array->count--;
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
    
    PGCString *join = PGCStringInstance();
    for (uint64_t i = 0; i < array->count; i++) {
        PGCStringAppendString(join, PGCDescription(PGCArrayGetObjectAtIndex(array, i)));
        if (i != array->count - 1) PGCStringAppendString(join, separator);
    }
    
    return join;
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
