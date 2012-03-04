//
//  PGCDictionaryEntry.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 3/3/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include "PGCDictionaryEntry.h"

struct _PGCDictionaryEntry {
    PGCObject super;
    PGCType key;
    PGCType object;
};

#pragma mark Private Function Interfaces

void PGCDictionaryEntryDealloc(PGCType instance);
uint64_t PGCDictionaryEntryHash(PGCType instance);


#pragma mark -

PGCClass *PGCDictionaryEntryClass(void)
{
    static PGCClass *dictionaryEntryClass = NULL;
    if (!dictionaryEntryClass) {
        PGCClassFunctions functions = { PGCDictionaryEntryCopy, PGCDictionaryEntryDealloc, NULL, PGCDictionaryEntryEquals, 
            PGCDictionaryEntryHash, NULL, NULL };
        dictionaryEntryClass = PGCClassCreate("PGCDictionaryEntry", PGCObjectClass(), functions, sizeof(PGCDictionaryEntry));
    }
    return dictionaryEntryClass;    
}


PGCDictionaryEntry *PGCDictionaryEntryInstanceWithObjectAndKey(PGCType object, PGCType key)
{
    return PGCAutorelease(PGCDictionaryEntryInitWithObjectAndKey(NULL, object, key));
}


PGCDictionaryEntry *PGCDictionaryEntryInitWithObjectAndKey(PGCDictionaryEntry *entry, PGCType object, PGCType key)
{
    if (!object || !key) return NULL;
    if (!entry && (entry = PGCAlloc(PGCDictionaryEntryClass())) == NULL) return NULL;

    PGCObjectInit(&entry->super);
    entry->key = PGCRetain(key);
    entry->object = PGCRetain(object);    
    return entry;
}


void PGCDictionaryEntryDealloc(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDictionaryEntryClass())) return;
    PGCDictionaryEntry *entry = instance;
    PGCRelease(entry->key);
    PGCRelease(entry->object);
    PGCSuperclassDealloc(entry);
}


bool PGCDictionaryEntryEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1 || !instance2) return false;
    if (!PGCObjectIsKindOfClass(instance1, PGCDictionaryEntryClass()) || !PGCObjectIsKindOfClass(instance2, PGCDictionaryEntryClass())) return false;

    PGCDictionaryEntry *entry1 = instance1;
    PGCDictionaryEntry *entry2 = instance2;

    return PGCEquals(entry1->key, entry2->key) && PGCEquals(entry1->object, entry2->object);
}


uint64_t PGCDictionaryEntryHash(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDictionaryEntryClass())) return 0;
    return PGCHash(((PGCDictionaryEntry *)instance)->key) * 33587 + PGCHash(((PGCDictionaryEntry *)instance)->object);
}


PGCType PGCDictionaryEntryCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCDictionaryEntryClass())) return NULL;
    PGCDictionaryEntry *entry = instance;
    return PGCDictionaryEntryInitWithObjectAndKey(entry, entry->object, entry->key);
}


PGCType PGCDictionaryEntryGetKey(PGCDictionaryEntry *entry)
{
    return entry ? PGCAutorelease(PGCRetain(entry->key)) : NULL;
}


PGCType PGCDictionaryEntryGetObject(PGCDictionaryEntry *entry)
{
    return entry ? PGCAutorelease(PGCRetain(entry->object)) : NULL;
}


void PGCDictionaryEntrySetObject(PGCDictionaryEntry *entry, PGCType object)
{
    if (!entry || !object || entry->object == object) return;
    PGCRelease(entry->object);
    entry->object = PGCRetain(object);
}


bool PGCDictionaryEntryKeyEquals(PGCDictionaryEntry *entry, PGCType key)
{
    return entry && key ? PGCEquals(entry->key, key) : false;
}
