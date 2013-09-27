//
//  PGCBase.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/24/2012.
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

#ifndef PGCBASE_H
#define PGCBASE_H

/*!
 @header PGCBase
 @discussion The PGCBase header defines the base types, constants, and polymorphic functions in the PGCFoundation API. 
 
     Of particular importance are the mechanisms for polymorphism that are defined. @link PGCType @/link and the various 
     polymorphic functions are used throughout the API to refer to objects and operate on them in a polymorphic way. For example, 
     @link PGCRetain @/link always invokes the appropriate Retain function for a particular object based on the object’s class. 
     Thus, calling PGCRetain is almost always preferable to calling a class-specific Retain function implementation, as the code
     will be shorter and more maintainable.
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#pragma mark Types and Data Structures

/*!
 @abstract The base type for PGCFoundation objects. 
 @discussion PGCType should be used when a function argument or return value can by polymorphic, as it eliminates the need for type  
     casts. Generally, in these cases, the real type of the object can be surmised by examining its class, though it is often enough
     to simply treat them as PGCObject instances.
 */
typedef void *PGCType;

/*!
 @abstract The type for the PGCString class.
 @discussion Except when subclassing PGCString, users should always use pointers to PGCStrings. This type is defined here instead of 
     in @link PGCString.h @/link because the PGCString needs to be defined for Description functions.
 */
typedef struct _PGCString PGCString;

/*!
 @abstract A special value to denote that some object or value could not be found. 
 @discussion PGCNotFound is typically returned by a function, e.g., @link PGCArrayGetIndexOfObject @/link, when a search failed to find
     any results.
 */
extern const uint64_t PGCNotFound;

/*!
 @abstract The range type, which stores the starting point and length of a discrete range.
 @field location The starting location of the range.
 @field length The length of the range.
 @discussion Ranges are primarily useful for expressing ranges of indices in strings, lists, and arrays. A PGCRange with a location
     of PGCNotFound and a length of 0 typically denotes an invalid range.
 */
typedef struct _PGCRange PGCRange;
struct _PGCRange {
    uint64_t location;
    uint64_t length;
}; 

/*!
 @abstract A pointer to a Copy class function.
 @param instance The object instance to copy.
 @result A copy of the specified instance with a retain count of 1; returns NULL if instance is NULL or otherwise invalid.
 @discussion See @link PGCClassFunctions @/link for more info on class functions.
 */
typedef PGCType PGCCopyFunction(PGCType instance);

/*!
 @abstract A pointer to a Dealloc class function.
 @param instance The object to deallocate.
 @discussion See @link PGCClassFunctions @/link for more info on class functions.
 */
typedef void PGCDeallocFunction(PGCType instance);

/*!
 @abstract A pointer to a Description class function.
 @param instance The object for which to get a description.
 @result A string that describes the object; returns NULL if instance is NULL or another error occurred.
 @discussion See @link PGCClassFunctions @/link for more info on class functions.
*/
typedef PGCString *PGCDescriptionFunction(PGCType instance);

/*!
 @abstract A pointer to an Equals class function.
 @param instance1 One of the objects whose equality is being tested.
 @param instance2 The other object whose equality is being tested.
 @result A boolean indicating whether the two objects are equal; returns false is either object is NULL or are otherwise incomparable.
 @discussion See @link PGCClassFunctions @/link for more info on class functions.
 */
typedef bool PGCEqualsFunction(PGCType instance1, PGCType instance2);

/*!
 @abstract A pointer to a Hash class function.
 @param instance The object for which to get a hash value.
 @result The hash value of the specified object; returns 0 if instance is NULL or another error occurred.
 @discussion See @link PGCClassFunctions @/link for more info on class functions.
 */
typedef uint64_t PGCHashFunction(PGCType instance);

/*!
 @abstract A pointer to a Release class function.
 @param instance The object instance to release.
 @discussion See @link PGCClassFunctions @/link for more info on class functions.
 */
typedef void PGCReleaseFunction(PGCType instance);

/*!
 @abstract A pointer to a Retain class function.
 @param instance The object to retain.
 @result The object that was retained; returns NULL if instance is NULL.
 @discussion See @link PGCClassFunctions @/link for more info on class functions.
 */
typedef PGCType PGCRetainFunction(PGCType instance);


#pragma mark Range functions

extern PGCRange PGCMakeRange(uint64_t location, uint64_t length);


#pragma mark Polymorphic Functions

/*!
 @abstract Returns a copy of the specified object.
 @param instance The object to copy.
 @result A copy of the specified object with a retain count of 1; returns NULL if instance is NULL or otherwise invalid.
 @discussion The appropriate Copy function for the specified object is polymorphically invoked based on its class. See 
     @link PGCClassFunctions @/link for more information about class functions.
 */
extern PGCType PGCCopy(PGCType instance);

/*!
 @abstract Frees up any resources used by the specfied object.
 @param instance The object to deallocate.
 @discussion The appropriate Dealloc function for the specified object is polymorphically invoked based on its class. See 
     @link PGCClassFunctions @/link for more information about class functions.
 */
extern void PGCDealloc(PGCType instance);

/*!
 @abstract Returns a description of the specified object.
 @param instance The object for which to get a description.
 @result A string that describes the object; returns NULL if instance is NULL or another error occurred.
 @discussion The appropriate Description function for the specified object is polymorphically invoked based on its class. See 
     @link PGCClassFunctions @/link for more information about class functions.
 */
extern PGCString *PGCDescription(PGCType instance);

/*!
 @abstract Convenience function that returns a description of the specified object as a C string.
 @param instance The object for which to get a description.
 @result A C string that describes the object; returns NULL if instance is NULL or another error occurred.
 @discussion This function works by invoking @link PGCStringGetCString @/link with the output of @link PGCDescription @/link. As
     such, the memory for the returned C string does not belong to the caller. If a copy of the C string is required or the string
     needs to persist beyond the next time the current autorelease pool is destroyed, the caller should call strdup on this 
     function's return value.
 */ 
extern const char *PGCDescriptionCString(PGCType instance);

/*!
 @abstract Returns whether two objects are equal.
 @param instance1 One of the objects whose equality is being tested.
 @param instance2 The other object whose equality is being tested.
 @result A boolean indicating whether the two objects are equal; returns false is either object is NULL, or they are otherwise incomparable.
 @discussion The appropriate Equals function for the specified objects is polymorphically invoked based on the class of instance1.
     See @link PGCClassFunctions @/link for more information about class functions.
 */
extern bool PGCEquals(PGCType instance1, PGCType instance2);

/*!
 @abstract Returns a hash of the specified object.
 @param instance The object for which to get a hash value.
 @result The hash value of the specified object; returns 0 if instance is NULL or another error occurred.
 @discussion The appropriate Hash function for the specified object is polymorphically invoked based on its class. See 
     @link PGCClassFunctions @/link for more information about class functions.
 */
extern uint64_t PGCHash(PGCType instance);


/*!
 @abstract Retains the specified object and returns it.
 @param instance The object to retain.
 @result The object that was retained; returns NULL if instance is NULL.
 @discussion The appropriate Retain function for the specified object is polymorphically invoked based on its class. See 
     @link PGCClassFunctions @/link for more information about class functions.
 */
extern PGCType PGCRetain(PGCType instance);

/*!
 @abstract Releases the specified object.
 @param instance The object to release.
 @discussion The appropriate Release function for the specified object is polymorphically invoked based on its class. See 
     @link PGCClassFunctions @/link for more information about class functions.
 */
extern void PGCRelease(PGCType instance);

/*!
 @abstract Adds the specified object to the current thread’s top-most autorelease pool.
 @param instance The object to autorelease.
 @result The object that was autoreleased; returns NULL if instance is NULL.
 @discussion This function works by calling PGCAutoreleasePoolAddObject with instance as its argument. It then returns
     instance. See @link PGCAutoreleasePool @/link for more information on autoreleasing objects.
 */
extern PGCType PGCAutorelease(PGCType instance);


#pragma mark Polymorphic Superclass Functions

/*!
 @abstract Invokes the specified object’s superclass’s Copy function.
 @discussion This function is merely provided as a convenience for subclasses. It should never be invoked directly.
 */
extern PGCType PGCSuperclassCopy(PGCType instance);

/*!
 @abstract Invokes the specified object’s superclass’s Dealloc function.
 @discussion This function is merely provided as a convenience for subclasses. It should never be invoked directly.
 */
extern void PGCSuperclassDealloc(PGCType instance);

/*!
 @abstract Invokes the specified object’s superclass’s Description function.
 @discussion This function is merely provided as a convenience for subclasses. It should never be invoked directly.
 */
extern PGCString *PGCSuperclassDescription(PGCType instance);


/*!
 @abstract Invokes the specified object’s superclass’s Equals function.
 @discussion This function is merely provided as a convenience for subclasses. It should never be invoked directly.
 */
extern bool PGCSuperclassEquals(PGCType instance1, PGCType instance2);

/*!
 @abstract Invokes the specified object’s superclass’s Hash function.
 @discussion This function is merely provided as a convenience for subclasses. It should never be invoked directly.
 */
extern uint64_t PGCSuperclassHash(PGCType instance);

/*!
 @abstract Invokes the specified object’s superclass’s Release function.
 @discussion This function is merely provided as a convenience for subclasses. It should never be invoked directly.
 */
extern void PGCSuperclassRelease(PGCType instance);

/*!
 @abstract Invokes the specified object’s superclass’s Retain function.
 @discussion This function is merely provided as a convenience for subclasses. It should never be invoked directly.
 */
extern PGCType PGCSuperclassRetain(PGCType instance);


#pragma mark Enumeration

enum {
    PGCEnumerationConcurrent = 1UL << 0,
    PGCEnumerationReverse = 1UL << 1
};

typedef uint64_t PGCEnumerationOptions;

typedef void (^PGCIndexedEnumerationBlock)(PGCType object, uint64_t index, bool *stop);


#endif
