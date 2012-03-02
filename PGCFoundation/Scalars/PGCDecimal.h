//
//  PGCDecimal.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
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
     
     As is the case with all PGCFoundation classes, the internal fields of PGCDecimal should never be accessed directly. In
     fact, we would prefer to not even expose the field names, but we must in order to support structural subtyping. To get the
     values of a PGCDecimal’s instance variables, use its accessor functions.
 */
typedef struct _PGCDecimal PGCDecimal;
struct _PGCDecimal {
    PGCObject super;
    double value;
};

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
