//
//  PGCObject.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/22/2012.
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

#ifndef PGCOBJECT_H
#define PGCOBJECT_H

/*!
 @header PGCObject
 @discussion The PGCObject header defines PGCObject, which is a root class and the base class for all PGCFoundation objects.
     The PGCObject class serves three basic purposes. First, it works stores a reference to an object’s class, thus giving
     an object a dynamic type. Next, it implements manual garbage collection using reference counting. Finally, it provides
     default implementations for the Description, Equals, Hash, Retain, and Release functions that subclasses may inherit.
     Generally speaking, subclasses will want to override Description, Equals, and Hash. There is no default implementation
     of Copy.
 */

#include <PGCFoundation/PGCBase.h>
#include <PGCFoundation/PGCClass.h>
#include <PGCFoundation/PGCAutoreleasePool.h>

#pragma mark - PGCObject 

/*!
 @typedef PGCObject
 @abstract The PGCObject class, which is a root class and the base class for all PGCFoundation objects.
 @discussion Like other classes in PGCFoundation, all objects should be allocated on the heap using either PGCAlloc or the
     class’s initializer with a NULL first argument, which simply calls PGCAlloc for you. A raw PGCObject struct should never
     be used except as the first field of a direct subclass’s struct definition in order to support structural subtyping.
 
     Also, as is the case with all PGCFoundation classes, the internal fields of PGCObject should never be accessed directly. In
     fact, we would prefer to not even expose the field names, but we must in order to support structural subtyping. To get the
     instance variables of a PGCObject, use its accessor functions.
 */
typedef struct _PGCObject PGCObject;
struct _PGCObject {
    PGCClass *isa;
    uint64_t retainCount;
};

/*!
 @abstract Returns the class data structure for PGCObject.
 @result The class data structure for PGCObject.
 */
extern PGCClass *PGCObjectClass();

/*!
 @abstract Allocates a new instance of the specified class, which should be PGCObject or one of its subclasses.
 @param class The class of the instance which is to be allocated.
 @result A pointer to the newly allocated memory; returns NULL if class is not a subclass of PGCObject or if allocation failed.
 @discussion In addition to allocating space for the specified object, this function sets the isa pointer for the object to the
     correct class and sets the object’s reference count to 1. All other memory is zeroed out.
 
     While users are free to use this method to allocate instances of a given class, by convention they may simply invoke a class’s
     object initializer with a NULL first argument. For example, the following two lines are equivalent:

     <pre>         PGCString *string = PGCStringInitWithCString(PGCAlloc(PGCString()), "blah blah blah");</pre>
     and
     <pre>         PGCString *string = PGCStringInitWithCString(NULL, "blah blah blah");</pre>
     
     In the latter line, the initializer automatically allocates an instance because its first argument is NULL. As such, there is
     almost no need to invoke this method outside of an initializer.
*/
extern PGCType PGCAlloc(PGCClass *class);


#pragma mark Basic Functions

/*!
 @abstract Initializes the specified PGCObject.
 @param object The PGCObject to initialize.
 @result The PGCObject that was initialized; returns NULL if an error occurred during initialization.
 @discussion In reality, this function does nothing but return the argument that it was passed. That may change in the future, however, so 
     subclasses should call this initializer in their initializer functions. 
 */
extern PGCObject *PGCObjectInit(PGCObject *object);

/*!
 @abstract Returns a description of the specified object.
 @param instance The object for which to get a description.
 @result A string containing the object’s class and address in hexadecimal; returns NULL if instance is NULL or another error occurred.
 */
extern PGCString *PGCObjectDescription(PGCType instance);

/*!
 @abstract Returns whether two objects are equal.
 @param instance1 One of the objects whose equality is being tested.
 @param instance2 The other object whose equality is being tested.
 @result A boolean indicating whether the two objects are equal; returns false is either object is NULL, or they are otherwise incomparable.
 @discussion Two PGCObjects are equal if and only if they have the same pointer address.
 */
extern bool PGCObjectEquals(PGCType instance1, PGCType instance2);

/*!
 @abstract Returns a hash of the specified object.
 @param instance The object for which to get a hash value.
 @result The hash value of the specified object; returns 0 if instance is NULL or another error occurred.
 @discussion The hash of a PGCObject is simply its address cast as a 64-bit unsigned integer.
 */
extern uint64_t PGCObjectHash(PGCType instance);

/*!
 @abstract Releases the specified object by decrementing its retain count, and if the resulting retain count is 0, invokes its Dealloc function.
 @param instance The object to release.
 */
extern void PGCObjectRelease(PGCType instance);

/*!
 @abstract Retains the specified object by incrementing its retain count.
 @param instance The object to retain.
 @result The object that was retained; returns NULL if instance is NULL.
 */
extern PGCType PGCObjectRetain(PGCType instance);


#pragma mark Class Introspection

/*!
 @abstract Returns the object’s class.
 @param instance The object whose class is to be returned.
 @result The object’s class; returns NULL if instance is NULL.
 */
extern PGCClass *PGCObjectGetClass(PGCType instance);

/*!
 @abstract Returns whether the specified object is type-compatible with the specified class.
 @param instance The object
 @param class The class
 @result Whether object is type-compatible with the specified class; returns false if either instance or class is NULL.
 @discussion An object is type-compatible with a class if it is an instance of that class or its class is a subclass of that class.
 */
extern bool PGCObjectIsKindOfClass(PGCType instance, PGCClass *class);

#endif
