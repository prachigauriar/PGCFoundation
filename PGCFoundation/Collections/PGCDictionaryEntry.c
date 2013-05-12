//
//  PGCDictionaryEntry.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 3/3/2012.
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
        PGCClassFunctions functions = { PGCDictionaryEntryCopy, PGCDictionaryEntryDealloc, NULL, NULL, NULL, NULL, NULL };
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
    if (!PGCObjectIsKindOfClass(instance, PGCDictionaryEntryClass())) return;
    PGCDictionaryEntry *entry = instance;
    PGCRelease(entry->key);
    PGCRelease(entry->object);
    PGCSuperclassDealloc(entry);
}


PGCType PGCDictionaryEntryCopy(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCDictionaryEntryClass())) return NULL;
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
