//
//  PGCDecimal.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCDecimal.h>
#include <PGCFoundation/PGCString.h>

#include <stdio.h>

PGCClass *PGCDecimalClass(void)
{
    static PGCClass *decimalClass = NULL;
    if (!decimalClass) {
        PGCClassFunctions functions = { PGCDecimalCopy, NULL, PGCDecimalDescription, PGCDecimalEquals, PGCDecimalHash, NULL, NULL };
        decimalClass = PGCClassCreate("PGCDecimal", PGCObjectClass(), functions, sizeof(PGCDecimal));
    }
    return decimalClass;
}


PGCDecimal *PGCDecimalNewWithValue(double value)
{
    return PGCAutorelease(PGCDecimalInitWithValue(NULL, value));
}


#pragma mark Basic Functions

PGCDecimal *PGCDecimalInitWithValue(PGCDecimal *decimal, double value)
{
    if (!decimal && (decimal = PGCClassAllocateInstance(PGCDecimalClass())) == NULL) return NULL;
    PGCObjectInitWithClass(&decimal->super, PGCDecimalClass());
    decimal->value = value;
    return decimal;
}


PGCType PGCDecimalCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDecimalClass())) return NULL;
    return PGCDecimalInitWithValue(NULL, ((PGCDecimal *)instance)->value);    
}


PGCString *PGCDecimalDescription(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDecimalClass())) return NULL;
    return PGCStringNewWithFormat("%f", ((PGCDecimal *)instance)->value);
}


bool PGCDecimalEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1 || !instance2) return false;
    if (!PGCObjectIsKindOfClass(instance1, PGCDecimalClass()) || !PGCObjectIsKindOfClass(instance2, PGCDecimalClass())) return false;
    return ((PGCDecimal *)instance1)->value == ((PGCDecimal *)instance2)->value;
}


uint64_t PGCDecimalHash(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDecimalClass())) return 0;
    
    // Multiply by an arbitrary large prime before removing the fractional part so that all values of the form n.x don't map to n.
    return ((PGCDecimal *)instance)->value * 1248757;
}


#pragma mark Accessors

double PGCDecimalGetValue(PGCDecimal *decimal)
{
    return decimal ? decimal->value : 0;
}
