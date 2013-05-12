//
//  PGCDictionaryEntry.h
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
