//
//  PGCInteger.h
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

#ifndef PGCINTEGER_H
#define PGCINTEGER_H

/*!
 @header PGCInteger
 @discussion The PGCInteger header defines PGCInteger, which is a simple wrapper class for storing primitive C integers. 
 */

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCInteger

/*!
 @typedef PGCInteger
 @superclass PGCObject
 @abstract The PGCInteger class, which wraps primitive integer values.
 @discussion PGCInteger objects, or simply integers, are object wrappers for primitive C signed and unsigned integers. Its primary
     purpose is to enable the storage of primitive integers in PGCFoundation collections.
     
     The value of a PGCInteger can be gotten using @link PGCIntegerGetSignedValue @/link or @link PGCIntegerGetUnsignedValue @/link.
     
     PGCInteger is not meant to be subclassed. As such, we do not expose the details of its data structure.
 */
typedef struct _PGCInteger PGCInteger;

/*!
 @abstract Returns the class data structure for PGCInteger.
 @result The class data structure for PGCInteger.
 */
extern PGCClass *PGCIntegerClass(void);

/*!
 @abstract Returns a newly created, autoreleased PGCInteger object with the specified signed integer value.  
 @param value The signed integer value.
 @result A newly created, autoreleased PGCInteger object with the specified signed integer value; returns NULL if 
     creation failed.
 */
extern PGCInteger *PGCIntegerInstanceWithSignedValue(int64_t value);

/*!
 @abstract Returns a newly created, autoreleased PGCInteger object with the specified unsigned integer value.  
 @param value The unsigned integer value.
 @result A newly created, autoreleased PGCInteger object with the specified unsigned integer value; returns NULL if
     creation failed.
 */
extern PGCInteger *PGCIntegerInstanceWithUnsignedValue(uint64_t value);


#pragma mark Basic Functions

/*!
 @abstract Initializes the specified PGCInteger object with the specified signed integer value.
 @param integer The PGCInteger object to initialize
 @param value The signed integer value
 @result An initialized PGCInteger instance with the specified signed integer value; returns NULL if initialization
     failed.
 @discussion For convenience, if integer is NULL, this function will automatically allocate a new PGCInteger instance 
     and initializes it.
 */
extern PGCInteger *PGCIntegerInitWithSignedValue(PGCInteger *integer, int64_t value);

/*!
 @abstract Initializes the specified PGCInteger object with the specified unsigned integer value.
 @param integer The PGCInteger object to initialize
 @param value The unsigned integer value
 @result An initialized PGCInteger instance with the specified unsigned integer value; returns NULL if initialization
     failed.
 @discussion For convenience, if integer is NULL, this function will automatically allocate a new PGCInteger instance 
     and initializes it.
 */
extern PGCInteger *PGCIntegerInitWithUnsignedValue(PGCInteger *integer, uint64_t value);


/*!
 @abstract Returns a copy of the specified PGCInteger object.
 @param instance The PGCInteger object to copy.
 @result A copy of the specified PGCInteger object; returns NULL if instance is NULL or not a PGCInteger object or if 
     creating the copy failed.
 */
extern PGCType PGCIntegerCopy(PGCType instance);

/*!
 @abstract Returns a string containing the specified PGCInteger object’s value.
 @param instance The PGCInteger object for which to get a description.
 @result A string containing the specified PGCInteger object’s value; returns NULL if instance was NULL or not a PGCInteger object.
 */
extern PGCString *PGCIntegerDescription(PGCType instance);

/*!
 @abstract Returns whether the specified PGCInteger objects are equal.
 @param instance1 The first of the PGCInteger objects to check for equality.
 @param instance2 The second of the PGCInteger objects to check for equality.
 @result Whether the two PGCInteger objects have the same integer value; returns false if either instance is NULL or
     not a PGCInteger object.
 */
extern bool PGCIntegerEquals(PGCType instance1, PGCType instance2);

/*!
 @abstract Returns a hash value for the specified PGCInteger.
 @param instance The PGCInteger object for which to get a hash.
 @result The hash value of the specified PGCInteger object; returns 0 if instance is NULL or not a PGCInteger object.
 */
extern uint64_t PGCIntegerHash(PGCType instance);


#pragma mark Accessors

/*!
 @abstract Returns whether the specified PGCInteger has a signed integer value. 
 @param instance The PGCInteger object.
 @result Whether the specified PGCInteger has a signed integer value. This is not based on the sign of the object’s value,
     but instead on whether the integer was initialized with a signed integer value.
 */
extern bool PGCIntegerIsSigned(PGCInteger *integer);

/*!
 @abstract Returns whether the specified PGCInteger has an unsigned integer value. 
 @param instance The PGCInteger object.
 @result Whether the specified PGCInteger has an unsigned integer value. This is not based on the sign of the object’s value,
     but instead on whether the integer was initialized with an unsigned integer value.
 */
extern bool PGCIntegerIsUnsigned(PGCInteger *integer);

/*!
 @abstract Returns the value of the specified PGCInteger as a primitive signed integer.
 @param integer The PGCInteger object.
 @param The value of the specified PGCInteger as a primitive signed integer. If the integer object was initialized with an unsigned
     value, its value is cast to be a signed integer.
 */
extern int64_t PGCIntegerGetSignedValue(PGCInteger *integer);

/*!
 @abstract Returns the value of the specified PGCInteger as a primitive unsigned integer.
 @param integer The PGCInteger object.
 @param The value of the specified PGCInteger as a primitive unsigned integer. If the integer object was initialized with a signed
     value, its value is cast to be an unsigned integer.
 */
extern uint64_t PGCIntegerGetUnsignedValue(PGCInteger *integer);

#endif
