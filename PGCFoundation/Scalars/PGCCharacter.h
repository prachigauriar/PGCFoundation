//
//  PGCCharacter.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCCHARACTER_H
#define PGCCHARACTER_H

/*!
 @header PGCCharacter
 @discussion The PGCCharacter header defines PGCCharacter, which is a simple wrapper class for storing primitive C characters. 
 */

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCCharacter

/*!
 @typedef PGCCharacter
 @superclass PGCObject
 @abstract The PGCCharacter class, which wraps primitive character values.
 @discussion PGCCharacter objects, or simply characters, are object wrappers for primitive C characters. Its primary purpose is to
     enable the storage of primitive characters in PGCFoundation collections.
     
     The value of a PGCCharacter can be gotten using @link PGCCharacterGetValue @/link.

     PGCCharacter is not meant to be subclassed. As such, we do not expose the details of its data structure.
 */
typedef struct _PGCCharacter PGCCharacter;

/*!
 @abstract Returns the class data structure for PGCCharacter.
 @result The class data structure for PGCCharacter.
 */
extern PGCClass *PGCCharacterClass(void);

/*!
 @abstract Returns a newly created, autoreleased PGCCharacter object with the specified character value.  
 @param value The character value.
 @result A newly created, autoreleased PGCCharacter object with the specified character value; returns NULL if creation failed.
 */
extern PGCCharacter *PGCCharacterInstanceWithValue(char value);


#pragma mark Basic Functions

/*!
 @abstract Initializes the specified PGCCharacter object with the specified character value.
 @param character The PGCCharacter object to initialize
 @param value The character value
 @result An initialized PGCCharacter instance with the specified character value; returns NULL if initialization failed.
 @discussion For convenience, if character is NULL, this function will automatically allocate a new PGCCharacter instance 
     and initializes it.
 */
extern PGCCharacter *PGCCharacterInitWithValue(PGCCharacter *character, char value);

/*!
 @abstract Returns a copy of the specified PGCCharacter object.
 @param instance The PGCCharacter object to copy.
 @result A copy of the specified PGCCharacter object; returns NULL if instance is NULL or not a PGCCharacter object or if 
     creating the copy failed.
 */
extern PGCType PGCCharacterCopy(PGCType instance);

/*!
 @abstract Returns a string containing the specified PGCCharacter object’s value.
 @param instance The PGCCharacter object for which to get a description.
 @result A string containing the specified PGCCharacter object’s value; returns NULL if instance was NULL or not a PGCCharacter object.
 */
extern PGCString *PGCCharacterDescription(PGCType instance);

/*!
 @abstract Returns whether the specified PGCCharacter objects are equal.
 @param instance1 The first of the PGCCharacter objects to check for equality.
 @param instance2 The second of the PGCCharacter objects to check for equality.
 @result Whether the two PGCCharacter objects have the same character values; returns false if either instance is NULL or not
     a PGCCharacter object.
 */
extern bool PGCCharacterEquals(PGCType instance1, PGCType instance2);

/*!
 @abstract Returns a hash value for the specified PGCCharacter.
 @param instance The PGCCharacter object for which to get a hash.
 @result The hash value of the specified PGCCharacter object; returns 0 if instance is NULL or not a PGCCharacter object.
 */
extern uint64_t PGCCharacterHash(PGCType instance);


#pragma mark Accessors

/*!
 @abstract Returns the primtive character value of the specified PGCCharacter object.
 @param instance The PGCCharacter object.
 @result The primitive character value of the specified PGCCharacter object; returns 0 if instance is NULL.
 */
extern char PGCCharacterGetValue(PGCCharacter *character);

#endif
