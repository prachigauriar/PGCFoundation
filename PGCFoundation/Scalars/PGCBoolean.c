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

#pragma mark Private Function Interfaces

PGCBoolean *PGCBooleanInitWithValue(PGCBoolean *boolean, bool value);
void PGCBooleanDealloc(PGCType instance);
void PGCBooleanRelease(PGCType instance);
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
    static PGCBoolean *sharedTrue = NULL;
    if (!sharedTrue) sharedTrue = PGCBooleanInitWithValue(PGCClassAllocateInstance(PGCBooleanClass()), true);
    return sharedTrue;
}


PGCBoolean *PGCBooleanFalse(void)
{
    static PGCBoolean *sharedFalse = NULL;
    if (!sharedFalse) sharedFalse = PGCBooleanInitWithValue(PGCClassAllocateInstance(PGCBooleanClass()), false);
    return sharedFalse;
}


#pragma mark Basic Functions

PGCBoolean *PGCBooleanInitWithValue(PGCBoolean *boolean, bool value)
{
    // If the user didn't provide a boolean to initialize, give them either the shared true or false
    if (!boolean) return value ? PGCBooleanTrue() : PGCBooleanFalse();
    
    // Otherwise, initialize our superclass and set our value
    PGCObjectInitWithClass(&boolean->super, PGCBooleanClass());
    boolean->value = value;
    return boolean;
}


void PGCBooleanDealloc(PGCType instance)
{    
}


PGCType PGCBooleanCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCBooleanClass())) return NULL;
    return instance;
}


PGCString *PGCBooleanDescription(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCBooleanClass())) return NULL;
    return PGCStringNewWithCString((((PGCBoolean *)instance)->value ? "true" : "false"));
}


bool PGCBooleanEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1 || !instance2) return false;
    return PGCObjectIsKindOfClass(instance1, PGCBooleanClass()) && instance1 == instance2;
}


uint64_t PGCBooleanHash(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCBooleanClass())) return 0;
    return ((PGCBoolean *)instance)->value ? 1299827 : 1299449;    // Two large arbitrary prime numbers
}


void PGCBooleanRelease(PGCType instance)
{
}


PGCType PGCBooleanRetain(PGCType instance)
{
    return instance;
}


#pragma mark Accessors

bool PGCBooleanGetValue(PGCBoolean *boolean)
{
    return boolean ? boolean->value : false;
}
