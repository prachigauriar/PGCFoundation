//
//  PGCInteger.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCInteger.h>
#include <PGCFoundation/PGCString.h>

#include <stdio.h>

/*!
 @struct _PGCInteger
 @abstract PGCInteger’s corresponding data structure.
 @field super The instance’s superclass’s fields.
 @field isSigned Whether the instance was initialized using a signed integer or not.
 @field value A union storing the instance’s value as either a signed value (signedValue) or an unsigned value (unsignedValue).
 */
struct _PGCInteger {
    PGCObject super;
    bool isSigned;
    union _PGCIntegerValue {
        int64_t signedValue;
        uint64_t unsignedValue;        
    } value;
};


PGCClass *PGCIntegerClass(void)
{
    static PGCClass *integerClass = NULL;
    if (!integerClass) {
        PGCClassFunctions functions = { PGCIntegerCopy, NULL, PGCIntegerDescription, PGCIntegerEquals, PGCIntegerHash, NULL, NULL };
        integerClass = PGCClassCreate("PGCInteger", PGCObjectClass(), functions, sizeof(PGCInteger));
    }
    return integerClass;
}


PGCInteger *PGCIntegerInstanceWithSignedValue(int64_t value)
{
    return PGCAutorelease(PGCIntegerInitWithSignedValue(NULL, value));
}


PGCInteger *PGCIntegerInstanceWithUnsignedValue(uint64_t value)
{
    return PGCAutorelease(PGCIntegerInitWithUnsignedValue(NULL, value));
}


#pragma mark Basic Functions

PGCInteger *PGCIntegerInitWithSignedValue(PGCInteger *integer, int64_t value)
{
    if (!integer && (integer = PGCAlloc(PGCIntegerClass())) == NULL) return NULL;
    PGCObjectInit(&integer->super);
    integer->isSigned = true;
    integer->value.signedValue = value;
    return integer;
}


PGCInteger *PGCIntegerInitWithUnsignedValue(PGCInteger *integer, uint64_t value)
{
    if (!integer && (integer = PGCAlloc(PGCIntegerClass())) == NULL) return NULL;
    PGCObjectInit(&integer->super);
    integer->isSigned = false;
    integer->value.unsignedValue = value;
    return integer;
}


PGCType PGCIntegerCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCIntegerClass())) return NULL;
    PGCInteger *integer = instance;
    if (integer->isSigned) {
        return PGCIntegerInitWithSignedValue(NULL, integer->value.signedValue);
    } else {
        return PGCIntegerInitWithUnsignedValue(NULL, integer->value.unsignedValue);
    }
}


PGCString *PGCIntegerDescription(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCIntegerClass())) return NULL;
    PGCInteger *integer = instance; 
    if (integer->isSigned) { 
        return PGCStringInstanceWithFormat("%lld", integer->value.signedValue); 
    } else {
        return PGCStringInstanceWithFormat("%llu", integer->value.unsignedValue);
    }
}


bool PGCIntegerEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1 || !instance2) return false;
    if (!PGCObjectIsKindOfClass(instance1, PGCIntegerClass()) || !PGCObjectIsKindOfClass(instance2, PGCIntegerClass())) return false;
    
    PGCInteger *integer1 = instance1;
    PGCInteger *integer2 = instance2;
    
    // If they have the same sign, they're equal if they have the same signed value
    if (integer1->isSigned == integer2->isSigned) return integer1->value.signedValue == integer2->value.signedValue;
    
    // The return statement below assumes integer1 is signed and integer2 isn't. If that's not the case, switch them around
    if (!integer1->isSigned) {
        integer1 = instance2;
        integer2 = instance1;
    } 
    
    // Since the signs differ, they are only equal if the signed one is non-negative, the unsigned one is <= int max, and their values are equal
    return integer1->value.signedValue >= 0 && integer2->value.unsignedValue <= INT64_MAX && integer1->value.signedValue == integer2->value.unsignedValue;
}


uint64_t PGCIntegerHash(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCIntegerClass())) return 0;
    
    // Multiply by an arbitrary large prime before removing the fractional part so that all values of the form n.x don't map to n.
    return ((PGCInteger *)instance)->value.unsignedValue;
}


#pragma mark Accessors

int64_t PGCIntegerGetSignedValue(PGCInteger *integer)
{
    return integer ? integer->value.signedValue : 0;
}


uint64_t PGCIntegerGetUnsignedValue(PGCInteger *integer)
{
    return integer ? integer->value.unsignedValue : 0;
}
