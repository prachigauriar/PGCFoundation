//
//  PGCArray.h
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

#ifndef PGCARRAY_H
#define PGCARRAY_H

#include <PGCFoundation/PGCObject.h>
#include <PGCFoundation/PGCString.h>

typedef struct _PGCArray PGCArray;

extern PGCClass *PGCArrayClass(void);
extern PGCArray *PGCArrayInstance(void);
extern PGCArray *PGCArrayWithObjects(PGCType object1, ...);

#pragma mark Basic Functions

extern PGCArray *PGCArrayInit(PGCArray *array);
extern PGCArray *PGCArrayInitWithInitialCapacity(PGCArray *array, uint64_t initialCapacity);
extern PGCArray *PGCArrayInitWithInitialCapacityAndIncrement(PGCArray *array, uint64_t initialCapacity, uint64_t increment);
extern PGCArray *PGCArrayInitWithObjects(PGCArray *array, PGCType object1, ...);

extern PGCType PGCArrayCopy(PGCType instance);
extern PGCString *PGCArrayDescription(PGCType instance);
extern bool PGCArrayEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCArrayHash(PGCType instance);

#pragma mark Accessors

extern uint64_t PGCArrayGetCount(PGCArray *array);
extern bool PGCArrayContainsObject(PGCArray *array, PGCType instance);

extern PGCType PGCArrayGetObjectAtIndex(PGCArray *array, uint64_t index);
extern PGCType PGCArrayGetFirstObject(PGCArray *array);
extern PGCType PGCArrayGetLastObject(PGCArray *array);

extern uint64_t PGCArrayGetIndexOfObject(PGCArray *array, PGCType instance);
extern uint64_t PGCArrayGetIndexOfObjectInRange(PGCArray *array, PGCType instance, PGCRange range);
extern uint64_t PGCArrayGetIndexOfIdenticalObject(PGCArray *array, PGCType instance);
extern uint64_t PGCArrayGetIndexOfIdenticalObjectInRange(PGCArray *array, PGCType instance, PGCRange range);

#pragma mark Subarrays

extern PGCArray *PGCArraySubarrayWithRange(PGCArray *array, PGCRange range);

#pragma mark Object Addition, Replacement, and Removal

extern void PGCArrayAddObject(PGCArray *array, PGCType instance);
extern void PGCArrayInsertObjectAtIndex(PGCArray *array, PGCType instance, uint64_t index);
extern void PGCArrayPushObject(PGCArray *array, PGCType instance);

extern void PGCArrayExchangeValuesAtIndices(PGCArray *array, uint64_t index1, uint64_t index2);

extern void PGCArrayReplaceObjectAtIndex(PGCArray *array, PGCType instance, uint64_t index);

extern PGCType PGCArrayPopObject(PGCArray *array);
extern void PGCArrayRemoveObject(PGCArray *array, PGCType instance);
extern void PGCArrayRemoveObjectAtIndex(PGCArray *array, uint64_t index);
extern void PGCArrayRemoveAllObjects(PGCArray *array);

#pragma mark String Conversion

extern PGCString *PGCArrayJoinComponentsWithString(PGCArray *array, PGCString *separator);


#pragma mark Memory Efficiency

extern void PGCArrayCondense(PGCArray *array);

#pragma mark Enumeration

extern void PGCArrayEnumerateObjectsWithBlock(PGCArray *array, PGCEnumerationOptions options, PGCIndexedEnumerationBlock block);

#endif
