//
//  PGCClass.h
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

#ifndef PGCFoundation_PGCClass_h
#define PGCFoundation_PGCClass_h

/*!
 @header PGCClass
 @discussion The PGCClass header defines types and functions that enable a limited inheritance, polymorphic object system. Specifically,
     PGCClass provides a pseudoclass mechanism, in which a set of functions can be called polymorphically and overridden by derived
     pseudoclasses. It also provides a mechanism for allocating an instance of a class and checking whether a class is a subclass of 
     another.
 */

#include <PGCFoundation/PGCBase.h>

#pragma mark PGCClassFunctions

/*!
 @typedef PGCClassFunctions
 @abstract A set of function pointers to a class’s actual class function implementations.
 
 @field copy The class’s Copy function.
 @field dealloc The class’s Dealloc function.
 @field description The class’s Description function.
 @field equals The class’s Equals function.
 @field hash The class’s Hash function.
 @field release The class’s Release function.
 @field retain The class’s Retain function.
 
 @discussion The PGCClassFunctions data structure provides a convenient way to store pointers to a class’s functions. A NULL 
     function pointer implies that the class’s superclass implementation should be used. To denote that nothing should be done, a
     pointer to a function with an empty implementation should be used.
     
     Copy functions return a (typically shallow) copy of the specified instance. Like the Retain and Release functions, its
     implementation by a class is optional; however, some PGCFoundation classes require that an object’s class have a Copy function.
     For example, the PGCDictionary class requires that its keys all have Copy functions. When a Copy function returns, the retain
     count of the object it returned should be +1.
 
     Dealloc functions free up resources consumed by an object instance. Generally, this means releasing any reatained instance variables,
     freeing any manually allocated buffers, closing any files or network connections, and invoking the superclass’s Dealloc function.
     Dealloc should never be called directly on an object. Instead, PGCObject’s Release implementation will automatically call Dealloc on
     an object when the object’s retain count reaches 0.
 
     Description functions return a PGCString representation of an object. These are typically useful for logging or debugging.
 
     Equals functions return whether two objects are equal.
 
     Hash functions return a hash value for an object, which should be suitable for use by a dictionary or other hashing data structure.
     The implementation of a class’s Equals and Hash functions should be such that if the Equals function returns true when given two 
     objects, the result of their Hash functions should be identical. That is, equality of objects implies equality of hashes. Note that
     the converse is not true: equality of hashes does not imply equality of objects. This property can be exploited by hashing data
     structures to improve performance. Given the frequency that Hash is called by such data structures, Hash functions should be very
     fast, i.e., with a linear time upper bound on algorithmic complexity.

     The purpose of a Release function is to decrement an object’s retain count and invoke the Dealloc function when the retain count is 0.
     Classes that inherit from PGCObject, i.e., pretty much everything, will almost never need to provide their own implementation of Release.
     One notable exception is when a class uses the singleton pattern. In this case, an empty Release function will prevent an object from
     being deallocated, as its retain count will never reach 0.
 
     Retain functions increment the retain count of an object. As is the case with Release functions, classes that inherit from PGCObject
     will almost never need to provide their own implementation of Retain except when they use the singleton pattern. In that case, the 
     the implementation need only return the object being retained.
 */
typedef struct _PGCClassFunctions PGCClassFunctions;
struct _PGCClassFunctions {
    PGCCopyFunction *copy;
    PGCDeallocFunction *dealloc;
    PGCDescriptionFunction *description;
    PGCEqualsFunction *equals;
    PGCHashFunction *hash;
    PGCReleaseFunction *release;
    PGCRetainFunction *retain;
} ;


#pragma mark - PGCClass 

/*!
 @typedef PGCClass
 @abstract Class data structures store metadata about their associated class.
 @discussion Specific data stored includes the class’s name, a pointer to its superclass’s class data structure, its class functions
     and the size of one of its instances. Only one instance of a class data structure should exist for a given class. This is typically
     accomplished by storing a reference to the class in a static variable, which is assigned a value and never changed. For example:
 
     <pre>
     PGCClass *ThingClass() 
     {
         static PGCClass *thingClass = NULL;
         if (!thingClass) {
             PGCClassFunctions functions = { ThingCopy, ThingDealloc, ThingDescription, ThingEquals, ThingHash, NULL, NULL };
             thingClass = PGCClassCreate("Thing", PGCObjectClass(), functions, sizeof(Thing));
         }
         return thingClass;
     }
     </pre>
 
     Any code that needs a reference to the Thing class’s class data structure just calls ThingClass. 
 */
typedef struct _PGCClass PGCClass;


#pragma mark Creation and Deallocation

/*!
 @abstract Allocates and initializes a class.
 @param name The name of the class; may not be NULL.
 @param superclass The class’s superclass; NULL denotes a root class.
 @param functions The class’s class functions.
 @param instanceSize The size of an instance of the class.
 @result A newly allocated class data structure with the specified name, superclass, class functions, and instance size.
 @discussion A given class should never be created twice. All instances of a given class should use the same class data structure, else
     class introspection will not work correctly. See the @link PGCClass PGCClass type documentation @/link for the standard approach of
     accomplishing this.
 */
extern PGCClass *PGCClassCreate(const char *name, PGCClass *superclass, PGCClassFunctions functions, uint64_t instanceSize);

/*!
 @abstract Frees up the memory for the specified class data structure.
 @param class The class to free.
 @discussion This function is never actually called by PGCFoundation and probably never should be. It is provided for completeness 
     should a more thorough class unloading system be developed.
 */
extern void PGCClassDestroy(PGCClass *class);

#pragma mark Accessors

/*!
 @abstract Returns the name of the specified class.
 @param class The class.
 @result The name of the class; returns NULL if class was NULL.
 @discussion This function does not transfer ownership of its return value’s memory; it still belongs to the class itself.
 */
extern const char *PGCClassGetName(PGCClass *class);

/*!
 @abstract Returns the superclass of the specified class.
 @param class The class.
 @result The superclass of the class; returns NULL if class was NULL.
 */
extern PGCClass *PGCClassGetSuperclass(PGCClass *class);

/*!
 @abstract Returns the class functions for the specified class.
 @param class The class
 @result The class functions of the specified class; if class is NULL, all the function pointers in the returned data structure will be NULL.
 @discussion The function pointers returned are the function implementations that the class actually uses. As such, if a NULL function was 
     specified at class creation, a pointer to the inherited function implementation is returned in the data structure.
 */
extern PGCClassFunctions PGCClassGetClassFunctions(PGCClass *class);

/*!
 @abstract Returns the size of an instance of the specified class.
 @param class The class
 @result The size of an instance of the specified class or 0 if class is NULL.
 */
extern uint64_t PGCClassGetInstanceSize(PGCClass *class);


#pragma mark Instance Allocation

/*!
 @abstract Allocates and zeroes out memory for a new instance of the specified class.
 @param class The class of the instance which is to be allocated.
 @result A pointer to the newly allocated memory; returns NULL if class is NULL or allocation failed.
 @discussion Users should not call this function to allocate instances, instead use @link PGCAlloc @/link or use the implicit allocation
     convention of instance initializers. See the documentation of PGCAlloc for more details.
 */
extern PGCType PGCClassAllocateInstance(PGCClass *class);


#pragma mark Class Introspection

/*!
 @abstract Returns whether a class is a subclass of another class.
 @param class1 The potential subclass.
 @param class2 The potential superclass.
 @result Whether class1 is a subclass of class2, or false if either class1 or class2 is NULL.
 @discussion This function does not simply check that class1’s superclass is class2; it checks if class2 is an ancestor of
     class1 at any point in the class hierarchy.
 */
extern bool PGCClassIsSubclassOfClass(PGCClass *class1, PGCClass *class2);


#pragma mark Class Function Accessors

/*!
 @abstract Returns a pointer to the Copy function implementation for the specified class.
 @param class The class
 @result The Copy function for the specified class; returns NULL if class is NULL. The function returned may not be the 
     same as the function used to initialize the class. Specifically, if a NULL function was specified at class creation, 
     a pointer to the inherited function implementation is returned.
 */
extern PGCCopyFunction *PGCClassGetCopyFunction(PGCClass *class);

/*!
 @abstract Returns a pointer to the Dealloc function implementation for the specified class.
 @param class The class
 @result The Dealloc function for the specified class; returns NULL if class is NULL. The function returned may not be the
     same as the function used to initialize the class. Specifically, if a NULL function was specified at class creation, 
     a pointer to the inherited function implementation is returned.
 */
extern PGCDeallocFunction *PGCClassGetDeallocFunction(PGCClass *class);

/*!
 @abstract Returns a pointer to the Description function implementation for the specified class.
 @param class The class
 @result The Description function for the specified class; returns NULL if class is NULL. The function returned may not 
     be the same as the function used to initialize the class. Specifically, if a NULL function was specified at class
     creation, a pointer to the inherited function implementation is returned.
 */
extern PGCDescriptionFunction *PGCClassGetDescriptionFunction(PGCClass *class);

/*!
 @abstract Returns a pointer to the Equals function implementation for the specified class.
 @param class The class
 @result The Equals function for the specified class; returns NULL if class is NULL. The function returned may not be
     the same as the function used to initialize the class. Specifically, if a NULL function was specified at class
     creation, a pointer to the inherited function implementation is returned.
 */
extern PGCEqualsFunction *PGCClassGetEqualsFunction(PGCClass *class);

/*!
 @abstract Returns a pointer to the Hash function implementation for the specified class.
 @param class The class
 @result The Hash function for the specified class; returns NULL if class is NULL. The function returned may not be
     the same as the function used to initialize the class. Specifically, if a NULL function was specified at class
     creation, a pointer to the inherited function implementation is returned.
 */
extern PGCHashFunction *PGCClassGetHashFunction(PGCClass *class);

/*!
 @abstract Returns a pointer to the Release function implementation for the specified class.
 @param class The class
 @result The Release function for the specified class; returns NULL if class is NULL. The function returned may not be the
     same as the function used to initialize the class. Specifically, if a NULL function was specified at class creation, 
     a pointer to the inherited function implementation is returned.
 */
extern PGCReleaseFunction *PGCClassGetReleaseFunction(PGCClass *class);

/*!
 @abstract Returns a pointer to the Retain function implementation for the specified class.
 @param class The class
 @result The Retain function for the specified class; returns NULL if class is NULL. The function returned may not be the
     same as the function used to initialize the class. Specifically, if a NULL function was specified at class creation, 
     a pointer to the inherited function implementation is returned.
 */
extern PGCRetainFunction *PGCClassGetRetainFunction(PGCClass *class);

#endif
