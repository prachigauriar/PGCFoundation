//
//  PGCDictionary.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 3/3/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCDICTIONARY_H
#define PGCDICTIONARY_H

#include <PGCFoundation/PGCObject.h>
#include <PGCFoundation/PGCArray.h>

#pragma mark - PGCDictionary

typedef struct _PGCDictionary PGCDictionary;

extern PGCClass *PGCDictionaryClass(void);
extern PGCDictionary *PGCDictionaryInstance(void);
extern PGCDictionary *PGCDictionaryInstanceWithObjectsAndKeys(PGCType object, PGCType key, ...);

#pragma mark Basic Functions

extern PGCDictionary *PGCDictionaryInit(PGCDictionary *dictionary);
extern PGCDictionary *PGCDictionaryInitWithObjectsAndKeys(PGCDictionary *dictionary, PGCType object, PGCType key, ...);

extern PGCType PGCDictionaryCopy(PGCType instance);
extern PGCString *PGCDictionaryDescription(PGCType instance);
extern bool PGCDictionaryEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCDictionaryHash(PGCType instance);

#pragma mark Accessors

extern uint64_t PGCDictionaryGetCount(PGCDictionary *dictionary);
extern PGCType PGCDictionaryGetObjectForKey(PGCDictionary *dictionary, PGCType key);
extern void PGCDictionarySetObjectForKey(PGCDictionary *dictionary, PGCType object, PGCType key);
extern void PGCDictionaryRemoveObjectForKey(PGCDictionary *dictionary, PGCType key);
extern void PGCDictionaryRemoveAllObjects(PGCDictionary *dictionary);

extern PGCArray *PGCDictionaryGetAllKeys(PGCDictionary *dictionary);
extern PGCArray *PGCDictionaryGetAllValues(PGCDictionary *dictionary);

#endif
