//
//  PGCDecimal.h
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

#ifndef PGCDECIMAL_H
#define PGCDECIMAL_H

/*!
 @header PGCDecimal
 @discussion The PGCDecimal header defines PGCDecimal, which is a simple wrapper class for storing primitive C floating-point 
     numbers. 
 */

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCDecimal

/*!
 @typedef PGCDecimal
 @superclass PGCObject
 @abstract The PGCDecimal class, which wraps primitive floating-point values.
 @discussion PGCDecimal objects, or simply decimals, are object wrappers for primitive C floating-point numbers. Its primary
     purpose is to enable the storage of primitive floating-point numbers in PGCFoundation collections.
     
     The value of a PGCDecimal can be gotten using @link PGCDecimalGetValue @/link.
     
     PGCDecimal is not meant to be subclassed. As such, we do not expose the details of its data structure.
 */
typedef struct _PGCDecimal PGCDecimal;

/*!
 @abstract Returns the class data structure for PGCDecimal.
 @result The class data structure for PGCDecimal.
 */
extern PGCClass *PGCDecimalClass(void);

/*!
 @abstract Returns a newly created, autoreleased PGCDecimal object with the specified floating-point value.  
 @param value The floating-point value.
 @result A newly created, autoreleased PGCDecimal object with the specified floating-point value; returns NULL if creation failed.
 */
extern PGCDecimal *PGCDecimalInstanceWithValue(double value);


#pragma mark Basic Functions

/*!
 @abstract Initializes the specified PGCDecimal object with the specified floating-point value.
 @param decimal The PGCDecimal object to initialize
 @param value The floating-point value
 @result An initialized PGCDecimal instance with the specified floating-point value; returns NULL if initialization failed.
 @discussion For convenience, if decimal is NULL, this function will automatically allocate a new PGCDecimal instance 
     and initializes it.
 */
extern PGCDecimal *PGCDecimalInitWithValue(PGCDecimal *decimal, double value);

/*!
 @abstract Returns a copy of the specified PGCDecimal object.
 @param instance The PGCDecimal object to copy.
 result A copy of the specified PGCDecimal object; returns NULL if instance is NULL or not a PGCDecimal object or if creating the copy failed.
 */
extern PGCType PGCDecimalCopy(PGCType instance);

/*!
 @abstract Returns a string containing the specified PGCDecimal object’s value.
 @param instance The PGCDecimal object for which to get a description.
 @result A string containing the specified PGCDecimal object’s value; returns NULL if instance was NULL or not a PGCDecimal object.
 */
extern PGCString *PGCDecimalDescription(PGCType instance);

/*!
 @abstract Returns whether the specified PGCDecimal objects are equal.
 @param instance1 The first of the PGCDecimal objects to check for equality.
 @param instance2 The second of the PGCDecimal objects to check for equality.
 @result Whether the two PGCDecimals objects have the same floating-point values; returns false if either instance is NULL or
     not a PGCDecimal object.
 */
extern bool PGCDecimalEquals(PGCType instance1, PGCType instance2);

/*!
 @abstract Returns a hash value for the specified PGCDecimal.
 @param instance The PGCDecimal object for which to get a hash.
 @result The hash value of the specified PGCDecimal object; returns 0 if instance is NULL or not a PGCDecimal object.
 */
extern uint64_t PGCDecimalHash(PGCType instance);


#pragma mark Accessors

/*!
 @abstract Returns the primtive floating-point value of the specified PGCDecimal object.
 @param instance The PGCDecimal object.
 @result The primitive floating-point value of the specified PGCDecimal object; returns 0 if instance is NULL.
 */
extern double PGCDecimalGetValue(PGCDecimal *decimal);

#endif
