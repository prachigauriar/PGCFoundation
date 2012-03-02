//
//  PGCBoolean.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCBoolean.h>
#include <PGCFoundation/PGCString.h>

#include <string.h>

/*!
 @struct _PGCBoolean
 @abstract PGCBoolean’s corresponding data structure.
 @field super The instance’s superclass’s fields.
 @field value The boolean value for the instance.
 */
struct _PGCBoolean {
    PGCObject super;
    bool value;
};


#pragma mark Private Function Interfaces

/*!
 @abstract Initializes the specified PGCBoolean object with the specified value.
 @param boolean The PGCBoolean object to initialize.
 @param value The boolean value for the boolean object.
 @discussion Unlike many other initializers, if the initial argument is NULL, this function will not allocate a new instance. Instead
     it will return either the True or False shared instance based on the boolean value specified.
 */
PGCBoolean *PGCBooleanInitWithValue(PGCBoolean *boolean, bool value);

/*!
 @abstract Does nothing.
 @param instance The PGCBoolean object that would be deallocated, if this function did anything.
 @discussion This function does nothing because the class is a “doubleton,” and as such, we never want the allocated instances to be
     deallocated.
 */
void PGCBooleanDealloc(PGCType instance);

/*!
 @abstract Does nothing.
 @param instance The PGCBoolean object to release, if this function did anything.
 @discussion This function does nothing because the class is a “doubleton,” and as such, we never wants its reference count to reach 0.
 */
void PGCBooleanRelease(PGCType instance);

/*!
 @abstract Does nothing.
 @param instance The PGCBoolean object to retain, if this function did anything.
 @result The instance that was passed to it.
 @discussion This function does nothing because the class is a “doubleton,” and as such, we never wants its reference count to change.
 */
PGCType PGCBooleanRetain(PGCType instance);


#pragma mark - PGCBoolean 

PGCClass *PGCBooleanClass(void)
{
    static PGCClass *booleanClass = NULL;
    if (!booleanClass) {
        PGCClassFunctions functions = { PGCBooleanCopy, PGCBooleanDealloc, PGCBooleanDescription, PGCBooleanEquals, PGCBooleanHash, PGCBooleanRelease, PGCBooleanRetain };
        booleanClass = PGCClassCreate("PGCBoolean", PGCObjectClass(), functions, sizeof(PGCBoolean));
    }
    return booleanClass;
}


PGCBoolean *PGCBooleanTrue(void)
{
    static PGCBoolean *trueInstance = NULL;
    if (!trueInstance) trueInstance = PGCBooleanInitWithValue(PGCAlloc(PGCBooleanClass()), true);
    return trueInstance;
}


PGCBoolean *PGCBooleanFalse(void)
{
    static PGCBoolean *falseInstance = NULL;
    if (!falseInstance) falseInstance = PGCBooleanInitWithValue(PGCAlloc(PGCBooleanClass()), false);
    return falseInstance;
}


#pragma mark Basic Functions

PGCBoolean *PGCBooleanInitWithValue(PGCBoolean *boolean, bool value)
{
    // If the user didn't provide a boolean to initialize, give them either the shared true or false
    if (!boolean) return value ? PGCBooleanTrue() : PGCBooleanFalse();
    
    // Otherwise, initialize our superclass and set our value
    PGCObjectInit(&boolean->super);
    boolean->value = value;
    return boolean;
}


void PGCBooleanDealloc(PGCType instance)
{    
}


PGCType PGCBooleanCopy(PGCType instance)
{
    return (instance == PGCBooleanTrue() || instance == PGCBooleanFalse()) ? instance : NULL;
}


PGCString *PGCBooleanDescription(PGCType instance)
{
    if (instance == PGCBooleanTrue()) {
        return PGCStringInstanceWithCString("true");    
    } else if (instance == PGCBooleanFalse()) {
        return PGCStringInstanceWithCString("false");
    }
    
    return NULL;
}


bool PGCBooleanEquals(PGCType instance1, PGCType instance2)
{
    return instance1 == instance2 && (instance1 == PGCBooleanTrue() || instance1 == PGCBooleanFalse());
}


uint64_t PGCBooleanHash(PGCType instance)
{
    if (instance == PGCBooleanTrue()) {
        return 1299827;
    } else if (instance == PGCBooleanFalse()) {
        return 1299449;
    }
    
    return 0;
}


void PGCBooleanRelease(PGCType instance)
{
}


PGCType PGCBooleanRetain(PGCType instance)
{
    return instance;
}


#pragma mark Accessors

bool PGCBooleanIsTrue(PGCBoolean *boolean)
{
    return boolean == PGCBooleanTrue();
}


bool PGCBooleanIsFalse(PGCBoolean *boolean)
{
    return boolean == PGCBooleanFalse();    
}


bool PGCBooleanGetValue(PGCBoolean *boolean)
{
    return boolean ? boolean->value : false;
}
