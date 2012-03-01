//
//  PGCAutoreleasePool.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCAUTORELEASEPOOL_H
#define PGCAUTORELEASEPOOL_H

/*!
 @header PGCAutoreleasePool
 @discussion The PGCAutoreleasePool header defines types and functions for interacting with PGCFoundation’s autorelease pools. 
     Autorelease pools aim to simplify reference-counted memory management by automatically release objects at some well-defined point
     in the future. Objects that have been autoreleased are added to an autorelease pool, which releases each of the objects inside it
     when it is destroyed. Objects that are added to an autorelease pool multiple times are released once for each time they appear in
     the pool.
 
     The ultimate effect of using autorelease pools is that in many cases, e.g., when one needs the return value of a function for
     a very short amount of time, no memory management is necessary. If a function returns an autoreleased object and there are no 
     memory management errors, the object will have a non-zero retain count and thus will not be deallocated. However, it will be 
     released when the current autorelease pool is destroyed, and thus the object’s retains and releases will be balanced.
 
     PGCFoundation uses autorelease pools extensively. As such, any thread that uses PGCFoundation must create an autorelease pool, and
     at least one autorelease pool should exist during any PGCFoundation API call. To handle the case of having multiple autorelease pools
     at the same time, each thread actually has a stack of pools. When a pool is created, it is pushed onto its thread’s stack; destroying 
     a pool destroys all the pools that are above it on the stack and then releases all of the objects in the pool in the order that they
     were added. A pool should always be destroyed on the same thread that it was created on.
 
     One time multiple pools may be desirable is while in a loop in which a lot of temporary object allocations are occurring. In that
     case, bookending each iteration of the loop with pool creation and destruction has the effect of freeing up autoreleased objects more
     quickly. For example:
 
     <pre>
     while (object != NULL) {
         PGCAutoreleasePool *innerPool = PGCAutoreleasePoolCreate();

         ...    // Some code that allocates a lot of objects
         
         object = NextObject();
         PGCAutoreleasePoolDestroy(innerPool);
     }
     </pre>
 
     Generally speaking, however, this is unnecessary.
 */

#include <PGCFoundation/PGCBase.h>

/*!
 @abstract The opaque type that corresponds to an autorelease pool.
 @discussion As with most of PGCFoundation, only pointers to PGCAutoreleasePools should be used.
 */
typedef struct _PGCAutoreleasePool PGCAutoreleasePool;

/*!
 @abstract Creates a new autorelease pool and adds it to the calling thread’s stack of pools.
 @result The pool that was created, or NULL if creation failed.
 @discussion The pool is placed on the top of the calling thread’s stack of autorelease pools. Subsequent calls to 
     @link PGCAutoreleasePoolAddObject @/link will use this newly created pool, provided there are no other intervening calls to
     this function.
 */
PGCAutoreleasePool *PGCAutoreleasePoolCreate(void);

/*!
 @abstract Adds the specified object to the calling thread’s top-most autorelease pool.
 @param instance The object to add to the autorelease pool.
 @discussion Does nothing if instance is NULL or no autorelase pool has been created on the current thread.
 */
void PGCAutoreleasePoolAddObject(PGCType instance);

/*!
 @abstract Releases each of the objects in the specified pool and removes it from the calling thread’s stack of pools.
 @param pool The pool to destroy.
 @discussion If the specified pool is not at the top the calling thread’s stack of pools, each of the pools above it on
     the stack are destroyed before it releases the objects inside it.
 */
void PGCAutoreleasePoolDestroy(PGCAutoreleasePool *pool);

#endif
