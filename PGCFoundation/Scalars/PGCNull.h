//
//  PGCNull.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCNULL_H
#define PGCNULL_H

/*!
 @header PGCNull
 @discussion The PGCNull header defines PGCNull, a singleton class for representing null values in PGCFoundation collections. 
 */

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCNull

/*!
 @typedef PGCNull
 @superclass PGCObject
 @abstract The PGCNull class, a singleton class for representing null values in PGCFoundation collections.
 @discussion The PGCNull class is a singleton class whose primary purpose is to represent null values in PGCFoundation collections.
     Because of the semantics in reference-counted collections, storing a literal NULL pointer can be problematic, and is thus 
     disallowed. Instead, users should use the PGCNull instance. This single instance can be acquired using 
     @link PGCNullInstance @/link. A PGCNull object should never be allocated using PGCAlloc. In fact, the class has no public
     initializer function, so an object allocated in this way couldn’t be initialized anyway. 
 
     PGCNull is not meant to be subclassed. As such, we do not expose the details of its data structure.
 */
typedef struct _PGCNull PGCNull;

/*!
 @abstract Returns the class data structure for PGCNull.
 @result The class data structure for PGCNull.
 */
extern PGCClass *PGCNullClass(void);

/*!
 @abstract Returns the shared PGCNull instance.
 @result The shared PGCNull instance.
 */
extern PGCNull *PGCNullInstance(void);


#pragma mark Basic Functions

/*!
 @abstract Returns a copy of the specified PGCNull instance.
 @param instance The PGCNull object to copy.
 @result A copy of the specified PGCNull instance; returns NULL if the instance is NULL or not a PGCNull object.
 @discussion Given that PGCNull is a singleton, its Copy does not return an actual copy of the specified instance. Rather, it 
     simply returns the shared PGCNull instance.
 */
extern PGCType PGCNullCopy(PGCType instance);

/*!
 @abstract Returns a string representation of the specified PGCNull object.
 @param instance The PGCNull instance for which to get a description.
 @result "null" if instance is the shared PGCNull instance; returns NULL otherwise.
 */
extern PGCString *PGCNullDescription(PGCType instance);

/*!
 @abstract Returns whether the specified PGCNull objects are equal.
 @param instance1 The first of the PGCNull objects to check for equality.
 @param instance2 The second of the PGCNull objects to check for equality.
 @result Whether the two objects are equal; returns false if either instance is NULL or not a PGCNull object.
 */
extern bool PGCNullEquals(PGCType instance1, PGCType instance2);

/*!
 @abstract Returns a hash value for the specified PGCNull object.
 @param instance The PGCNull object for which to get a hash.
 @result The hash value of the specified PGCNull object; returns 0 if instance is NULL or not a PGCNull object.
 */
extern uint64_t PGCNullHash(PGCType instance);

#endif
