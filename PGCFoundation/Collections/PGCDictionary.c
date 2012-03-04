//
//  PGCDictionary.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 3/3/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCDictionary.h>
#include <PGCFoundation/PGCList.h>

#include "PGCDictionaryEntry.h"

struct _PGCDictionary {
    PGCObject super;
    PGCList **buckets;
    uint64_t count;
};

const uint64_t PGCDictionaryBucketCount = 512;

PGCDictionary *PGCDictionaryInitWithObjectAndKeyAndArguments(PGCDictionary *dictionary, PGCType object, PGCType key, va_list arguments);
PGCDictionaryEntry *PGCDictionaryGetEntryForKey(PGCList *bucket, PGCType key); 
void PGCDictionaryDealloc(PGCType instance);

#pragma mark -

PGCClass *PGCDictionaryClass(void)
{
    static PGCClass *dictionaryClass = NULL;
    if (!dictionaryClass) {
        PGCClassFunctions functions = { PGCDictionaryCopy, PGCDictionaryDealloc, PGCDictionaryDescription, 
            PGCDictionaryEquals, PGCDictionaryHash, NULL, NULL };
        dictionaryClass = PGCClassCreate("PGCDictionary", PGCObjectClass(), functions, sizeof(PGCDictionary));
    }
    return dictionaryClass;    
}


PGCDictionary *PGCDictionaryInstance(void)
{
    return PGCAutorelease(PGCDictionaryInit(NULL));
}


PGCDictionary *PGCDictionaryInstanceWithObjectsAndKeys(PGCType object, PGCType key, ...)
{
    va_list arguments;
    
    va_start(arguments, key);
    PGCDictionary *dictionary = PGCDictionaryInitWithObjectAndKeyAndArguments(NULL, object, key, arguments);    
    va_end(arguments);
    
    return PGCAutorelease(dictionary);
}


#pragma mark Basic Functions

PGCDictionary *PGCDictionaryInit(PGCDictionary *dictionary)
{
    if (!dictionary && (dictionary = PGCAlloc(PGCDictionaryClass())) == NULL) return NULL;
    
    PGCObjectInit(&dictionary->super);
    
    dictionary->buckets = calloc(PGCDictionaryBucketCount, sizeof(PGCList *));
    if (!dictionary->buckets) {
        PGCRelease(dictionary);
        return NULL;
    }
    
    return dictionary;
}


extern PGCDictionary *PGCDictionaryInitWithObjectsAndKeys(PGCDictionary *dictionary, PGCType object, PGCType key, ...)
{
    va_list arguments;
    
    va_start(arguments, key);
    dictionary = PGCDictionaryInitWithObjectAndKeyAndArguments(dictionary, object, key, arguments);    
    va_end(arguments);
    
    return dictionary;
}


PGCDictionary *PGCDictionaryInitWithObjectAndKeyAndArguments(PGCDictionary *dictionary, PGCType firstObject, PGCType firstKey, va_list arguments)
{
    dictionary = PGCDictionaryInit(dictionary);
    if (!dictionary) return NULL;
    
    PGCType object = firstObject;
    PGCType key = firstKey;
    
    while (object && key) {
        PGCDictionarySetObjectForKey(dictionary, object, key);
        object = va_arg(arguments, PGCType);
        key = object ? va_arg(arguments, PGCType) : NULL; 
    }
    
    return dictionary;
}


void PGCDictionaryDealloc(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDictionaryClass())) return;
    PGCDictionary *dictionary = instance;
    
    if (dictionary->buckets) {
        for (uint64_t i = 0; i < PGCDictionaryBucketCount; i++) PGCRelease(dictionary->buckets[i]);
        free(dictionary->buckets);
    }
    
    PGCSuperclassDealloc(dictionary);
}


PGCType PGCDictionaryCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDictionaryClass())) return NULL;
    PGCDictionary *dictionary = instance;
    
    PGCDictionary *copy = PGCDictionaryInit(NULL);
    if (!copy) return NULL;
    
    // Rather than simply calling PGCDictionarySetObjectForKey for each of our keys, we duplicate each of the buckets manually.
    // This is a lot more trouble, but should save a ton of memory and hashing. By creating copies of each entry, we forgo copying
    // the key and instead retain it. This is okay, because no one outside of a dictionary has a reference to the key, and thus it
    // can’t be modified out from underneath us. Also, because we have the buckets already set up, we don’t waste time recalculating
    // the hash.
    for (uint64_t i = 0; i < PGCDictionaryBucketCount; i++) {
        if (!dictionary->buckets[i]) continue;

        copy->buckets[i] = PGCListInit(NULL);
        if (!copy->buckets[i]) {
            PGCRelease(copy);
            return NULL;
        }

        // Copy each of the dictionary entries in this bucket
        uint64_t entryCount = PGCListGetCount(dictionary->buckets[i]);
        for (uint64_t j = 0; j < entryCount; j++) {
            PGCDictionaryEntry *entryCopy = PGCCopy(PGCListGetObjectAtIndex(dictionary->buckets[i], j));
            if (!entryCopy) {
                PGCRelease(copy);
                return NULL;
            }
            
            PGCListAddObject(copy->buckets[i], entryCopy);
        }
    }
    
    copy->count = dictionary->count;
    
    return copy;
}


PGCString *PGCDictionaryDescription(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDictionaryClass())) return NULL;
//    PGCDictionary *dictionary = instance;

    return NULL;
}


bool PGCDictionaryEquals(PGCType instance1, PGCType instance2)
{
    return false;
}


uint64_t PGCDictionaryHash(PGCType instance)
{
    return 0;
}


uint64_t PGCDictionaryGetCount(PGCDictionary *dictionary)
{
    return dictionary ? dictionary->count : 0;
}


PGCDictionaryEntry *PGCDictionaryGetEntryForKey(PGCList *bucket, PGCType key)
{
    if (!bucket || !key) return NULL;
    
    uint64_t entryCount = PGCListGetCount(bucket);
    for (uint64_t i = 0; i < entryCount; i++) {
        PGCDictionaryEntry *entry = PGCListGetObjectAtIndex(bucket, i);
        if (PGCDictionaryEntryKeyEquals(entry, key)) return entry;
    }
    
    return NULL;
}


PGCType PGCDictionaryGetObjectForKey(PGCDictionary *dictionary, PGCType key)
{
    if (!dictionary || !key) return NULL;
    PGCList *bucket = dictionary->buckets[PGCHash(key) % PGCDictionaryBucketCount];
    return PGCDictionaryEntryGetObject(PGCDictionaryGetEntryForKey(bucket, key));
}


void PGCDictionarySetObjectForKey(PGCDictionary *dictionary, PGCType object, PGCType key)
{
    if (!dictionary || !object || !key) return;
    uint64_t bucketIndex = PGCHash(key) % PGCDictionaryBucketCount;
    
    // Allocate a bucket if necessary
    PGCList *bucket = dictionary->buckets[bucketIndex]; 
    if (!bucket) dictionary->buckets[bucketIndex] = bucket = PGCListInit(NULL);
    if (!bucket) return;

    // Make a copy of the key
    PGCType keyCopy = PGCCopy(key);
    if (!keyCopy) return;
    
    // If we already have an entry for key in our bucket, just set its object to our key copy. Otherwise 
    // create a new entry, add it to the bucket, and increment our count
    PGCDictionaryEntry *entry = PGCDictionaryGetEntryForKey(bucket, keyCopy);
    if (entry) {
        PGCDictionaryEntrySetObject(entry, object);
    } else if ((entry = PGCDictionaryEntryInitWithObjectAndKey(NULL, object, keyCopy))) {
        PGCListAddObject(bucket, entry);
        dictionary->count++;
        PGCRelease(entry);
    }
    
    // Release the key copy we created (the bucket retained it)
    PGCRelease(keyCopy);
}


void PGCDictionaryRemoveObjectForKey(PGCDictionary *dictionary, PGCType key)
{
    if (!dictionary || !key) return;
    
    PGCList *bucket = dictionary->buckets[PGCHash(key) % PGCDictionaryBucketCount];
    uint64_t entryCount = PGCListGetCount(bucket);
    for (uint64_t i = 0; i < entryCount; i++) {
        PGCDictionaryEntry *entry = PGCListGetObjectAtIndex(bucket, i);
        if (!PGCDictionaryEntryKeyEquals(entry, key)) continue;
        
        PGCListRemoveObjectAtIndex(bucket, i);
        dictionary->count--;
    }
}


void PGCDictionaryRemoveAllObjects(PGCDictionary *dictionary)
{
    if (!dictionary) return;
    for (uint64_t i = 0; i < PGCDictionaryBucketCount; i++) PGCListRemoveAllObjects(dictionary->buckets[i]);
}


PGCArray *PGCDictionaryGetAllKeys(PGCDictionary *dictionary)
{
    PGCArray *allKeys = PGCArrayInitWithInitialCapacity(NULL, dictionary->count);
    if (!allKeys) return NULL;
    
    for (uint64_t i = 0; i < PGCDictionaryBucketCount; i++) {
        PGCList *bucket = dictionary->buckets[i];
        if (!bucket) continue;

        uint64_t entryCount = PGCListGetCount(bucket);
        for (uint64_t j = 0; j < entryCount; j++) {
            PGCArrayAddObject(allKeys, PGCDictionaryEntryGetKey(PGCListGetObjectAtIndex(bucket, j)));
        }
    }
    
    return PGCAutorelease(allKeys);
}


PGCArray *PGCDictionaryGetAllObjects(PGCDictionary *dictionary)
{
    PGCArray *allObjects = PGCArrayInitWithInitialCapacity(NULL, dictionary->count);
    if (!allObjects) return NULL;
    
    for (uint64_t i = 0; i < PGCDictionaryBucketCount; i++) {
        PGCList *bucket = dictionary->buckets[i];
        if (!bucket) continue;
        
        uint64_t entryCount = PGCListGetCount(bucket);
        for (uint64_t j = 0; j < entryCount; j++) {
            PGCArrayAddObject(allObjects, PGCDictionaryEntryGetObject(PGCListGetObjectAtIndex(bucket, j)));
        }
    }
    
    return PGCAutorelease(allObjects);
}
