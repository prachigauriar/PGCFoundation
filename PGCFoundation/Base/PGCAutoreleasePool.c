//
//  PGCAutoreleasePool.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCAutoreleasePool.h>
#include <PGCFoundation/PGCObject.h>

#include <pthread.h>

#pragma mark Private Global Variables

static bool PGCAutoreleasePoolThreadKeyInitialized = false;
static pthread_key_t PGCAutoreleasePoolThreadKey;


#pragma mark Private Data Types

typedef struct _PGCAutoreleasePoolEntry PGCAutoreleasePoolEntry;

struct _PGCAutoreleasePoolEntry {
    PGCAutoreleasePoolEntry *next;
    PGCType object;
};

struct _PGCAutoreleasePool {
    PGCAutoreleasePool *next;
    PGCAutoreleasePoolEntry *entriesHead;
    PGCAutoreleasePoolEntry *entriesTail;
};


#pragma mark Private Function Interfaces

void PGCAutoreleasePoolThreadWasDestroyed(void *threadVariable);


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
    pool->next = currentThreadPool;
    
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
    
    // Allocate a new instance and set its object to instance
    PGCAutoreleasePoolEntry *entry = calloc(1, sizeof(PGCAutoreleasePoolEntry));
    if (!entry) return;
    entry->object = instance;
    
    // Add our object to the pool's entry list
    // If the pool has no entries, make this entry the first one
    if (!pool->entriesHead) pool->entriesHead = entry;
    
    // If the pool has a tail, put our entry after it
    if (pool->entriesTail) pool->entriesTail->next = entry;
    
    // Make our new entry the end of the entry list
    pool->entriesTail = entry;
}


void PGCAutoreleasePoolDestroy(PGCAutoreleasePool *pool)
{
    // If we haven't set up a pool or the specified pool is NULL, return
    if (!PGCAutoreleasePoolThreadKeyInitialized || !pool) return;
    
    // Get the pool at the top of this thread's stack of pools
    PGCAutoreleasePool *topPool = pthread_getspecific(PGCAutoreleasePoolThreadKey);
    
    // As long we're not the top of the pool stack, we need to drain all the pools above us on the stack
    while (topPool != pool) {
        PGCAutoreleasePoolDestroy(topPool);
        topPool = pthread_getspecific(PGCAutoreleasePoolThreadKey);
    }

    // Release each entry's object 
    PGCAutoreleasePoolEntry *entry = pool->entriesHead;
    while (entry) {
        PGCRelease(entry->object);
        PGCAutoreleasePoolEntry *nextEntry = entry->next;
        free(entry);
        entry = nextEntry;
    }
    
    // Pop our pool off the stack and free up our memory
    pthread_setspecific(PGCAutoreleasePoolThreadKey, pool->next);
    free(pool);
}


void PGCAutoreleasePoolThreadWasDestroyed(void *threadVariable)
{
    PGCAutoreleasePoolDestroy(threadVariable);
}