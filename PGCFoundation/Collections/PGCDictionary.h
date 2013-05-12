//
//  PGCDictionary.h
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
