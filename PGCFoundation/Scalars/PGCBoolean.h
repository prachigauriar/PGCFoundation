//
//  PGCBoolean.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCBOOLEAN_H
#define PGCBOOLEAN_H

/*!
 @header PGCBoolean
 @discussion The PGCBoolean header defines PGCBoolean, which is a simple wrapper class for storing primitive C booleans. 
 */

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCBoolean

/*!
 @typedef PGCBoolean
 @superclass PGCObject
 @abstract The PGCBoolean class, which wraps primitive boolean values.
 @discussion PGCBoolean objects, or simply booleans, are object wrappers for primitive C booleans. Its primary purpose is to enable
     the storage of primitive booleans in PGCFoundation collections.
 
     The PGCBoolean class is implemented as a “doubleton;” there are only ever two valid instances of PGCBoolean: one representing
     true and one representing false. You can get these instances using @link PGCBooleanTrue @/link and @link PGCBooleanFalse @/link,
     respectively. A boolean object should never be allocated using PGCAlloc. In fact, the class has no public initializer function, 
     so an object allocated in this way couldn’t be initialized anyway. 
 
     The value of a PGCBoolean can be gotten using @link PGCBooleanGetValue @/link, though @link PGCBooleanIsTrue @/link and 
     @link PGCBooleanIsFalse @/link provide similar functionality.
 
     PGCBoolean is not meant to be subclassed. As such, we do not expose the details of its data structure.
 */
typedef struct _PGCBoolean PGCBoolean;

/*!
 @abstract Returns the class data structure for PGBoolean.
 @result The class data structure for PGCBoolean.
 */
extern PGCClass *PGCBooleanClass(void);

/*!
 @abstract Returns the PGCBoolean object representing true.
 @result The PGCBoolean object representing true.
 */
extern PGCBoolean *PGCBooleanTrue(void);

/*!
 @abstract Returns the PGCBoolean object representing false.
 @result The PGCBoolean object representing false.
 */
extern PGCBoolean *PGCBooleanFalse(void);


#pragma mark Basic Functions

/*!
 @abstract Returns a copy of the specified PGCBoolean.
 @param instance The PGCBoolean object to copy.
 @result A copy of the specified PGCBoolean instance; returns NULL if the instance is NULL or not a PGCBoolean object.
 @discussion Given that PGCBoolean is a “doubleton,” its Copy does not return an actual copy of the specified instance. Rather, it 
     simply returns the instance itself if it is a PGCBoolean object.
 */
extern PGCType PGCBooleanCopy(PGCType instance);

/*!
 @abstract Returns a string representation of the specfied PGCBoolean.
 @param instance The PGCBoolean object for which to get a description.
 @result "true" if the instance is the True boolean object, "false" if it is the False one; NULL otherwise. 
 */
extern PGCString *PGCBooleanDescription(PGCType instance);

/*!
 @abstract Returns whether the specified PGCBoolean objects are equal.
 @param instance1 The first of the PGCBoolean objects to check for equality.
 @param instance2 The second of the PGCBoolean objects to check for equality.
 @result Whether the two objects are equal; returns false if either instance is NULL or not a PGCBoolean object.
 */
extern bool PGCBooleanEquals(PGCType instance1, PGCType instance2);

/*!
 @abstract Returns a hash value for the specified PGCBoolean.
 @param instance The PGCBoolean object for which to get a hash.
 @result The hash value of the specified PGCBoolean object; returns 0 if instance is NULL or not a PGCBoolean object.
 */
extern uint64_t PGCBooleanHash(PGCType instance);


#pragma mark Accessors

/*!
 @abstract Returns whether the specified PGCBoolean object has a value of true.
 @param instance The PGCBoolean object whose truth value is to be checked.
 @result Whether the specified PGCBoolean object has a value of true.
 */
extern bool PGCBooleanIsTrue(PGCBoolean *boolean);

/*!
 @abstract Returns whether the specified PGCBoolean object has a value of false.
 @param instance The PGCBoolean object whose truth value is to be checked.
 @result Whether the specified PGCBoolean object has a value of false.
 */
extern bool PGCBooleanIsFalse(PGCBoolean *boolean);

/*!
 @abstract Returns the primtive boolean value of the specified PGCBoolean object.
 @param instance The PGCBoolean object.
 @result The primitive boolean value of the specified PGCBoolean object; returns false if instance is NULL.
 */
extern bool PGCBooleanGetValue(PGCBoolean *boolean);

#endif
