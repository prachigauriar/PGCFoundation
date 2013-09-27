//
//  PGCAutoreleasePool.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
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

#include <PGCFoundation/PGCAutoreleasePool.h>
#include <PGCFoundation/PGCObject.h>

#include <pthread.h>

#pragma mark Private Global Variables

/*!
 @abstract Indicates whether the key for storing thread-specific autorelease pool stacks has been initialized.
 @discussion This variable is set to true the first time PGCAutoreleasePoolCreate is called, provided that the thread key
     could be successfully created. It is never set to false.
 */
static bool PGCAutoreleasePoolThreadKeyInitialized = false;

/*!
 @abstract The thread key used to store thread-specific autorelease stacks. 
 @discussion This variable is initialized the first time PGCAutoreleasePoolCreate is called, provided that the thread key
     could be successfully created. It is never destroyed.
 */
static pthread_key_t PGCAutoreleasePoolThreadKey;


#pragma mark Private Types and Data Structures

/*!
 @typedef PGCAutoreleasePoolEntry
 @abstract A singly-linked list node used to store objects in an autorelease pool.
 @field next A pointer to the next node in the linked list; NULL if this node is the last item in the list.
 @field object The object in the autorelease pool.
 @discussion These structures are allocated in PGCAutoreleasePoolAddObject and freed in PGCAutoreleasePoolDestroy.
 */
typedef struct _PGCAutoreleasePoolEntry PGCAutoreleasePoolEntry;
struct _PGCAutoreleasePoolEntry {
    PGCAutoreleasePoolEntry *next;
    PGCType object;
};

/*!
 @abstract A doubly-linked stack node used to store autorelease pools.
 @field previous A pointer to the previous pool in the stack; NULL if this node is at the top of the stack. 
 @field next A pointer to the next pool in the stack; NULL if this node is at the bottom of the stack. 
 @field entriesHead A pointer to the first autorelease pool entry in the pool’s objects to eventually release; NULL
     if the pool contains no objects.
 @field entriesHead A pointer to the last autorelease pool entry in the pool’s objects to eventually release; NULL
     if the pool contains no objects.
 @discussion The previous and next pointers are only modified by PGCAutoreleasePoolCreate and PGCAutoreleasePoolDestroy.
     The entriesHead and entriesTail pointers are modified by PGCAutoreleasePoolAddObject. 
 */
struct _PGCAutoreleasePool {
    PGCAutoreleasePool *previous;
    PGCAutoreleasePool *next;
    PGCAutoreleasePoolEntry *entriesHead;
    PGCAutoreleasePoolEntry *entriesTail;
};


#pragma mark Private Function Interfaces

/*!
 @abstract Cleans up the autorelease pool stack when a thread terminates.
 @param threadVariable The terminating thread’s value for PGCAutoreleasePoolThreadKey, which is of type PGCAutoreleasePool.
 @discussion If threadVariable is non-NULL, this function simply iterates over each of the pools in the specified stack
     and essentially destroys them in the order they were placed on the autorelease pool stack.
 */
void PGCAutoreleasePoolThreadWasDestroyed(void *threadVariable);


// FIXME: Add logging
#pragma mark -

PGCAutoreleasePool *PGCAutoreleasePoolCreate(void)
{
    // Attempt to create the thread-specific key if it's not already set
    if (!PGCAutoreleasePoolThreadKeyInitialized && pthread_key_create(&PGCAutoreleasePoolThreadKey, PGCAutoreleasePoolThreadWasDestroyed) != 0) return NULL;
    PGCAutoreleasePoolThreadKeyInitialized = true;
    
    // Allocate a new pool
    PGCAutoreleasePool *pool = calloc(1, sizeof(PGCAutoreleasePool));
    if (!pool) return NULL;
    
    // Put the new pool at the top of the stack
    PGCAutoreleasePool *currentThreadPool = pthread_getspecific(PGCAutoreleasePoolThreadKey);
    if (currentThreadPool) {
        pool->next = currentThreadPool;
        currentThreadPool->previous = pool;
    }
    
    // Set our thread-specific pool to point to this one
    pthread_setspecific(PGCAutoreleasePoolThreadKey, pool);
    return pool;
}


void PGCAutoreleasePoolAddObject(PGCType instance)
{
    // If we haven't set up a pool or the instance was NULL, return
    if (!PGCAutoreleasePoolThreadKeyInitialized || !instance) return;

    // Get the thread-specific pool
    PGCAutoreleasePool *pool = pthread_getspecific(PGCAutoreleasePoolThreadKey);
    if (!pool) return;
    
    // Allocate a new entry and set its object to instance
    PGCAutoreleasePoolEntry *entry = calloc(1, sizeof(PGCAutoreleasePoolEntry));
    if (!entry) return;
    entry->object = instance;
    
    // Add our object to the pool's entry list
    if (!pool->entriesHead) {
        // If the pool has no entries, make this entry the first one
        pool->entriesHead = entry;
    } else {
        // Set our entry as the entry after the current tail
        pool->entriesTail->next = entry;
    }
    
    // Make our new entry the end of the entry list
    pool->entriesTail = entry;
}


void PGCAutoreleasePoolDestroy(PGCAutoreleasePool *pool)
{
    // If we haven't set up a pool or the specified pool is NULL, return
    if (!PGCAutoreleasePoolThreadKeyInitialized || !pool) return;
    
    // As long we're not the top of the pool stack, recursively destroy all the pools above us
    if (pool->previous) PGCAutoreleasePoolDestroy(pool->previous);

    // Release each entry's object 
    PGCAutoreleasePoolEntry *entry = pool->entriesHead;
    while (entry) {
        PGCRelease(entry->object);
        PGCAutoreleasePoolEntry *nextEntry = entry->next;
        free(entry);
        entry = nextEntry;
    }
    
    // Pop our pool off the stack and free up our memory
    if (pool->next) pool->next->previous = NULL;
    pthread_setspecific(PGCAutoreleasePoolThreadKey, pool->next);
    free(pool);
}


void PGCAutoreleasePoolThreadWasDestroyed(void *threadVariable)
{
    PGCAutoreleasePool *pool = threadVariable;
    while (pool) {
        PGCAutoreleasePool *nextPool = pool->next;

        // Below is effectively most of the body of PGCAutoreleasePoolDestroy without the call to pthread_setspecific, 
        // as calling pthread_setspecific in the middle of a thread-specific destructor “may result in lost storage or
        // infinite loops,” neither of which is desirable.
        
        // Release each entry's object 
        PGCAutoreleasePoolEntry *entry = pool->entriesHead;
        while (entry) {
            PGCRelease(entry->object);
            PGCAutoreleasePoolEntry *nextEntry = entry->next;
            free(entry);
            entry = nextEntry;
        }
        
        // Free the pool itself
        free(pool);
        pool = nextPool;
    }
}