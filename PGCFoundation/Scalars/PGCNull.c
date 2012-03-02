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

/*!
 @struct _PGCNull
 @abstract PGCNull’s corresponding data structure.
 @field super The instance’s superclass’s fields.
 @discussion This is about as minimal as a PGCObject subclass can be. Nothing ever get referenced within this class.
 */
struct _PGCNull {
    PGCObject super;
};


#pragma mark Private Function Interfaces

/*!
 @abstract Does nothing.
 @param instance The PGCNull object that would be deallocated, if this function did anything.
 @discussion This function does nothing because the class is a singleton, and as such, we never want the allocated instance to be
     deallocated.
 */
void PGCNullDealloc(PGCType instance);

/*!
 @abstract Does nothing.
 @param instance The PGCNull object to release, if this function did anything.
 @discussion This function does nothing because the class is a singleton, and as such, we never wants its reference count to reach 0.
 */
void PGCNullRelease(PGCType instance);

/*!
 @abstract Does nothing.
 @param instance The PGCNull object to retain, if this function did anything.
 @result The instance that was passed to it.
 @discussion This function does nothing because the class is a singleton, and as such, we never wants its reference count to change.
 */
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
        sharedInstance = PGCAlloc(PGCNullClass());
        PGCObjectInit(&sharedInstance->super);   
    }
    return sharedInstance;
}


#pragma mark Basic Functions

void PGCNullDealloc(PGCType instance)
{    
}


PGCType PGCNullCopy(PGCType instance)
{
    return instance == PGCNullInstance() ? instance : NULL;
}


PGCString *PGCNullDescription(PGCType instance)
{
    return instance == PGCNullInstance() ? PGCStringInstanceWithCString("null") : NULL;
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

