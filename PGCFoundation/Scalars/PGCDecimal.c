//
//  PGCDecimal.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
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

#include <PGCFoundation/PGCDecimal.h>
#include <PGCFoundation/PGCString.h>

#include <stdio.h>

/*!
 @struct _PGCDecimal
 @abstract PGCDecimal’s corresponding data structure.
 @field super The instance’s superclass’s fields.
 @field value The floating-point value for the instance.
 */
struct _PGCDecimal {
    PGCObject super;
    double value;
};

PGCClass *PGCDecimalClass(void)
{
    static PGCClass *decimalClass = NULL;
    if (!decimalClass) {
        PGCClassFunctions functions = { PGCDecimalCopy, NULL, PGCDecimalDescription, PGCDecimalEquals, PGCDecimalHash, NULL, NULL };
        decimalClass = PGCClassCreate("PGCDecimal", PGCObjectClass(), functions, sizeof(PGCDecimal));
    }
    return decimalClass;
}


PGCDecimal *PGCDecimalInstanceWithValue(double value)
{
    return PGCAutorelease(PGCDecimalInitWithValue(NULL, value));
}


#pragma mark Basic Functions

PGCDecimal *PGCDecimalInitWithValue(PGCDecimal *decimal, double value)
{
    if (!decimal && (decimal = PGCAlloc(PGCDecimalClass())) == NULL) return NULL;
    PGCObjectInit(&decimal->super);
    decimal->value = value;
    return decimal;
}


PGCType PGCDecimalCopy(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCDecimalClass())) return NULL;
    return PGCDecimalInitWithValue(NULL, ((PGCDecimal *)instance)->value);    
}


PGCString *PGCDecimalDescription(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCDecimalClass())) return NULL;
    return PGCStringInstanceWithFormat("%f", ((PGCDecimal *)instance)->value);
}


bool PGCDecimalEquals(PGCType instance1, PGCType instance2)
{
    if (!PGCObjectIsKindOfClass(instance1, PGCDecimalClass()) || !PGCObjectIsKindOfClass(instance2, PGCDecimalClass())) return false;
    return ((PGCDecimal *)instance1)->value == ((PGCDecimal *)instance2)->value;
}


uint64_t PGCDecimalHash(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCDecimalClass())) return 0;
    
    // Multiply by an arbitrary large prime before removing the fractional part so that all values of the form n.x don't map to n.
    return ((PGCDecimal *)instance)->value * 1248757;
}


#pragma mark Accessors

double PGCDecimalGetValue(PGCDecimal *decimal)
{
    return decimal ? decimal->value : 0;
}
