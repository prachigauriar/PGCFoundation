//
//  PGCString.c
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

#include <PGCFoundation/PGCString.h>

#include <ctype.h>
#include <string.h>
#include <stdio.h>


struct _PGCString {
    PGCObject super;
    
    char *buffer;
    uint64_t capacity;
    uint64_t length;
};

# pragma mark Private Global Constants

static const uint64_t PGCStringDefaultInitialCapacity = 128;


#pragma mark Private Function Interfaces

void PGCStringDealloc(PGCType instance);
void PGCStringReallocateBuffer(PGCString *string, uint64_t minimumCapacity);


#pragma mark -

PGCClass *PGCStringClass(void)
{
    static PGCClass *stringClass = NULL;
    if (!stringClass) {
        PGCClassFunctions functions = { PGCStringCopy, PGCStringDealloc, PGCStringDescription, PGCStringEquals, PGCStringHash, NULL, NULL };
        stringClass = PGCClassCreate("PGCString", PGCObjectClass(), functions, sizeof(PGCString));
    }
    return stringClass;
}


PGCString *PGCStringInstance(void)
{
    return PGCAutorelease(PGCStringInit(NULL));
}


PGCString *PGCStringInstanceWithCString(const char *cString)
{
    return PGCAutorelease(PGCStringInitWithCString(NULL, cString));
}


PGCString *PGCStringInstanceWithFormat(const char *format, ...)
{
    if (!format) return NULL;
    va_list arguments;
    
    va_start(arguments, format);
    PGCString *string = PGCStringInitWithFormatAndArguments(NULL, format, arguments);    
    va_end(arguments);
    
    return PGCAutorelease(string);
}


#pragma mark Basic Functions

PGCString *PGCStringInit(PGCString *string)
{
    if (!string && (string = PGCAlloc(PGCStringClass())) == NULL) return NULL;
    PGCObjectInit(&string->super);

    string->length = 0;
    string->capacity = PGCStringDefaultInitialCapacity;
    string->buffer = calloc(string->capacity, sizeof(char));
    if (!string->buffer) {
        PGCRelease(string);
        return NULL;
    }
    
    return string;
}


PGCString *PGCStringInitWithCString(PGCString *string, const char *cString)
{
    if (!cString) return PGCStringInit(string);
    if (!string && (string = PGCAlloc(PGCStringClass())) == NULL) return NULL;
    PGCObjectInit(&string->super);

    string->buffer = strdup(cString);
    string->length = strlen(string->buffer);
    string->capacity = string->length + 1;
    return string;
}


PGCString *PGCStringInitWithFormat(PGCString *string, const char *format, ...)
{
    va_list arguments;
    
    va_start(arguments, format);
    string = PGCStringInitWithFormatAndArguments(NULL, format, arguments);    
    va_end(arguments);
    
    return string;
}


PGCString *PGCStringInitWithFormatAndArguments(PGCString *string, const char *format, va_list arguments)
{
    if (!format) return NULL;

    uint64_t capacity = strlen(format) + PGCStringDefaultInitialCapacity;
    char *buffer = calloc(capacity, sizeof(char));

    // If we didn't have enough room in our buffer to accommodate the format string, 
    // reallocate and try again until it fits
    uint64_t requiredLength = vsnprintf(buffer, capacity, format, arguments);
    while (requiredLength >= capacity) {
        capacity = requiredLength + 1;
        char *reallocedBuffer = realloc(buffer, capacity * sizeof(char));
        if (!reallocedBuffer) return NULL;
        buffer = reallocedBuffer;
        requiredLength = vsnprintf(buffer, capacity, format, arguments);
    }
    
    string = PGCStringInitWithCString(string, buffer);
    free(buffer);
    return string;
}


void PGCStringDealloc(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCStringClass())) return;
    PGCString *string = instance;
    if (string->buffer) free(string->buffer);

    PGCSuperclassDealloc(string);
}


PGCType PGCStringCopy(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCStringClass())) return NULL;
    return PGCStringInitWithCString(NULL, ((PGCString *)instance)->buffer);
}


PGCString *PGCStringDescription(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCStringClass())) return NULL;
    return PGCAutorelease(PGCRetain(instance));
}


bool PGCStringEquals(PGCType instance1, PGCType instance2)
{
    if (!PGCObjectIsKindOfClass(instance1, PGCStringClass()) || !PGCObjectIsKindOfClass(instance2, PGCStringClass())) return false;
    return strcmp(((PGCString *)instance1)->buffer, ((PGCString *)instance2)->buffer) == 0;
}


uint64_t PGCStringHash(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCStringClass())) return 0;
    PGCString *string = instance;
    char *buffer = string->buffer;
    
    // This is the DJB2 hash function, which is a fast, public domain string hash with
    // excellent distribution. It was posted to comp.lang.c by Daniel J. Bernstein.
    uint64_t hash = 5381;
    char bufferIterator;
    while ((bufferIterator = *buffer++)) {
        // Set hash to hash * 33 + the current character's value
        hash = ((hash << 5) + hash) + bufferIterator;
    }
    
    return hash;
}


void PGCStringReallocateBuffer(PGCString *string, uint64_t minimumLength)
{
    if (!string || minimumLength <= string->length) return;
    
    // Keep doubling the new capacity until it exceeds the specified minimum length.
    // We need to strictly exceed the minimum length to allow for the NULL byte.
    uint64_t newCapacity = string->capacity;
    while (newCapacity <= minimumLength) newCapacity *= 2;
    
    // Reallocate our buffer
    char *reallocedBuffer = realloc(string->buffer, newCapacity * sizeof(char));
    if (!reallocedBuffer) return;
    
    // If reallocation succeeded, point to the new buffer and set our capacity to the new value
    string->buffer = reallocedBuffer;
    string->capacity = newCapacity;
}


#pragma mark Accessors

const char *PGCStringGetCString(PGCString *string)
{
    return string ? (const char *)string->buffer : NULL;
}


uint64_t PGCStringGetLength(PGCString *string)
{
    return string ? string->length : 0;
}


char PGCStringGetCharacterAtIndex(PGCString *string, uint64_t index)
{
    return (string && index < string->length) ? string->buffer[index] : 0;
}


void PGCStringSetCharacterAtIndex(PGCString *string, char character, uint64_t index)
{
    if (string && index < string->length) string->buffer[index] = character;
}


#pragma mark String Case

PGCString *PGCStringGetLowercaseString(PGCString *string)
{
    PGCString *lowercaseString = PGCCopy(string);
    if (!lowercaseString) return NULL;
    
    for (uint64_t i = 0; i < lowercaseString->length; i++) {
        lowercaseString->buffer[i] = tolower(lowercaseString->buffer[i]);
    }
    
    return PGCAutorelease(lowercaseString);
}


PGCString *PGCStringGetUppercaseString(PGCString *string)
{
    PGCString *uppercaseString = PGCCopy(string);
    if (!uppercaseString) return NULL;
    
    for (uint64_t i = 0; i < uppercaseString->length; i++) {
        uppercaseString->buffer[i] = toupper(uppercaseString->buffer[i]);
    }
    
    return PGCAutorelease(uppercaseString);
}


#pragma mark Substrings

PGCString *PGCStringGetSubstringToIndex(PGCString *string, uint64_t index)
{
    if (!string || index > string->length) return NULL;
    return PGCStringGetSubstringWithRange(string, PGCMakeRange(0, index));
}


PGCString *PGCStringGetSubstringWithRange(PGCString *string, PGCRange range)
{
    // If the string is invalid, or the range’s start is beyond the string, or the range’s overall
    // length goes beyond the end of the string, return NULL
    if (!string || range.location >= string->length || range.location + range.length > string->length) return NULL;
    
    char *buffer = calloc(range.length + 1, sizeof(char));
    if (!buffer) return NULL;
    
    // Copy range.length characters starting string->buffer[range.location] into our buffer, and create a new string using that buffer
    memcpy(buffer, &string->buffer[range.location], range.length * sizeof(char));
    PGCString *substring = PGCStringInstanceWithCString(buffer);
    
    free(buffer);
    return substring;
}


PGCString *PGCStringGetSubstringFromIndex(PGCString *string, uint64_t index)
{
    if (!string || index > string->length - 1) return NULL;
    return PGCStringGetSubstringWithRange(string,  PGCMakeRange(index, string->length - index));
}


bool PGCStringHasPrefix(PGCString *string, PGCString *prefix)
{
    if (!string || !prefix || prefix->length > string->length) return false;

    for (uint64_t i = 0; i < prefix->length; i++) {
        if (string->buffer[i] != prefix->buffer[i]) return false;
    }
    
    return true;
}


bool PGCStringHasSuffix(PGCString *string, PGCString *suffix)
{
    if (!string || !suffix || suffix->length > string->length) return false;

    uint64_t stringOffset = string->length - suffix->length;
    for (uint64_t i = 0; i < suffix->length; i++) {
        if (string->buffer[i + stringOffset] != suffix->buffer[i]) return false;
    }
    
    return true;    
}


#pragma mark String replacement

void PGCStringReplaceCharactersInRangeWithString(PGCString *string, PGCRange range, PGCString *replacementString)
{
    if (!string || !replacementString || range.location > string->length || range.location + range.length > string->length) return;

    // If the replacement string is bigger than the range being replaced, we need to resize
    int64_t lengthDifference = replacementString->length - range.length;
    if (lengthDifference > 0) {
        uint64_t minimumStringLength = string->length + lengthDifference;
        PGCStringReallocateBuffer(string, minimumStringLength);
        if (string->capacity <= minimumStringLength) return;
    }

    // Move everything after range to where it will be after the replacement
    uint64_t endOfRange = range.location + range.length;
    memmove(&string->buffer[range.location + replacementString->length], &string->buffer[endOfRange], (string->length - endOfRange) * sizeof(char));

    // Copy the contents of replacementString into the replacement range
    memmove(&string->buffer[range.location], &replacementString->buffer[0], replacementString->length * sizeof(char));
    
    string->length += lengthDifference;
    string->buffer[string->length] = '\0';
}


#pragma mark Inserting Strings

void PGCStringPrependString(PGCString *string, PGCString *prependString)
{
    PGCStringReplaceCharactersInRangeWithString(string, PGCMakeRange(0, 0), prependString);
}


void PGCStringInsertStringAtIndex(PGCString *string, PGCString *insertString, uint64_t index)
{
    PGCStringReplaceCharactersInRangeWithString(string, PGCMakeRange(index, 0), insertString);
}


void PGCStringAppendString(PGCString *string, PGCString *appendString)
{
    if (!string) return;
    PGCStringReplaceCharactersInRangeWithString(string, PGCMakeRange(string->length, 0), appendString);
}


void PGCStringAppendFormat(PGCString *string, const char *format, ...)
{
    if (!string || !format) return;
    va_list arguments;
    
    va_start(arguments, format);
    PGCString *formattedString = PGCStringInitWithFormatAndArguments(NULL, format, arguments);
    va_end(arguments);
    
    PGCStringAppendString(string, formattedString);
}


#pragma mark Memory Efficiency

void PGCStringCondense(PGCString *string)
{
    if (!string) return;

    // Reallocate our buffer
    char *reallocedBuffer = realloc(string->buffer, (string->length + 1) * sizeof(char));
    if (!reallocedBuffer) return;
    
    // If reallocation succeeded, point to the new buffer and set our capacity to the new value
    string->buffer = reallocedBuffer;
    string->capacity = string->length + 1;
}
