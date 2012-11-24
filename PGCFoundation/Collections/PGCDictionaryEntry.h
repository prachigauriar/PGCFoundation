//
//  PGCDictionaryEntry.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 3/3/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCDICTIONARYENTRY_H
#define PGCDICTIONARYENTRY_H

#include <PGCFoundation/PGCObject.h>

typedef struct _PGCDictionaryEntry PGCDictionaryEntry;

extern PGCClass *PGCDictionaryEntryClass(void);
extern PGCDictionaryEntry *PGCDictionaryEntryInstanceWithObjectAndKey(PGCType object, PGCType key);

extern PGCDictionaryEntry *PGCDictionaryEntryInitWithObjectAndKey(PGCDictionaryEntry *entry, PGCType object, PGCType key);
extern PGCType PGCDictionaryEntryCopy(PGCType instance);

extern PGCType PGCDictionaryEntryGetKey(PGCDictionaryEntry *entry);

extern PGCType PGCDictionaryEntryGetObject(PGCDictionaryEntry *entry);
extern void PGCDictionaryEntrySetObject(PGCDictionaryEntry *entry, PGCType object);

extern bool PGCDictionaryEntryKeyEquals(PGCDictionaryEntry *entry, PGCType key);

#endif
