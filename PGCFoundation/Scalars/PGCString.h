//
//  PGCString.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/24/2012.
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

#ifndef PGCSTRING_H
#define PGCSTRING_H

#include <PGCFoundation/PGCObject.h>
#include <PGCFoundation/PGCArray.h>

#pragma mark - PGCString

extern PGCClass *PGCStringClass(void);
extern PGCString *PGCStringInstance(void);
extern PGCString *PGCStringInstanceWithCString(const char *cString);
extern PGCString *PGCStringInstanceWithFormat(const char *format, ...);

#pragma mark Basic Functions

extern PGCString *PGCStringInit(PGCString *string);
extern PGCString *PGCStringInitWithCString(PGCString *string, const char *cString);
extern PGCString *PGCStringInitWithFormat(PGCString *string, const char *format, ...);
extern PGCString *PGCStringInitWithFormatAndArguments(PGCString *string, const char *format, va_list arguments);
extern PGCType PGCStringCopy(PGCType instance);
extern PGCString *PGCStringDescription(PGCType instance);
extern bool PGCStringEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCStringHash(PGCType instance);

#pragma mark Accessors

extern const char *PGCStringGetCString(PGCString *string);
extern uint64_t PGCStringGetLength(PGCString *string);
extern char PGCStringGetCharacterAtIndex(PGCString *string, uint64_t index);
extern void PGCStringSetCharacterAtIndex(PGCString *string, char character, uint64_t index);

#pragma mark String Case

extern PGCString *PGCStringGetLowercaseString(PGCString *string);
extern PGCString *PGCStringGetUppercaseString(PGCString *string);

//extern PGCArray *PGCSplitStringOnSeparator(PGCString *string, PGCString *separator);

#pragma mark Substrings

extern PGCString *PGCStringGetSubstringToIndex(PGCString *string, uint64_t index);
extern PGCString *PGCStringGetSubstringWithRange(PGCString *string, PGCRange range);
extern PGCString *PGCStringGetSubstringFromIndex(PGCString *string, uint64_t index);

extern bool PGCStringHasPrefix(PGCString *string, PGCString *prefix);
extern bool PGCStringHasSuffix(PGCString *string, PGCString *suffix);

#pragma mark String replacement

extern void PGCStringReplaceCharactersInRangeWithString(PGCString *string, PGCRange range, PGCString *replacementString);
//extern void PGCStringReplaceSubstringWithString(PGCString *string, PGCString *substring, PGCString *replacementString);

#pragma mark Inserting Strings

extern void PGCStringPrependString(PGCString *string, PGCString *prependString);
extern void PGCStringInsertStringAtIndex(PGCString *string, PGCString *insertString, uint64_t index);
extern void PGCStringAppendString(PGCString *string, PGCString *appendString);
extern void PGCStringAppendFormat(PGCString *string, const char *format, ...);

// PGCStringRemoveCharactersInRange

#endif
