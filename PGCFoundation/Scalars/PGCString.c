//
//  PGCString.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/24/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCString.h>

#include <string.h>
#include <stdio.h>

# pragma mark Private Global Constants

static const uint64_t PGCStringDefaultCapacity = 128;


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


PGCString *PGCStringNew(void)
{
    return PGCAutorelease(PGCStringInit(NULL));
}


PGCString *PGCStringNewWithCString(const char *cString)
{
    return PGCAutorelease(PGCStringInitWithCString(NULL, cString));
}


PGCString *PGCStringNewWithFormat(const char *format, ...)
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
    if (!string && (string = PGCClassAllocateInstance(PGCStringClass())) == NULL) return NULL;
    PGCObjectInitWithClass(&string->super, PGCStringClass());

    string->length = 0;
    string->capacity = PGCStringDefaultCapacity;
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
    if (!string && (string = PGCClassAllocateInstance(PGCStringClass())) == NULL) return NULL;
    PGCObjectInitWithClass(&string->super, PGCStringClass());

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

    uint64_t capacity = strlen(format) + PGCStringDefaultCapacity;
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
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCStringClass())) return;
    PGCString *string = instance;
    if (string->buffer) free(string->buffer);

    PGCSuperclassDealloc(string);
}


PGCType PGCStringCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCStringClass())) return NULL;
    return PGCStringInitWithCString(NULL, ((PGCString *)instance)->buffer);
}


PGCString *PGCStringDescription(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCStringClass())) return NULL;
    return PGCAutorelease(PGCRetain(instance));
}


bool PGCStringEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1 || !instance2) return false;
    if (!PGCObjectIsKindOfClass(instance1, PGCStringClass()) || !PGCObjectIsKindOfClass(instance2, PGCStringClass())) return false;
    return strcmp(((PGCString *)instance1)->buffer, ((PGCString *)instance2)->buffer) != 0;
}


uint64_t PGCStringHash(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCStringClass())) return 0;
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


#pragma mark Appending Strings

void PGCStringPrependString(PGCString *string, PGCString *prependString)
{
    PGCStringInsertStringAtIndex(string, prependString, 0);
}


void PGCStringInsertStringAtIndex(PGCString *string, PGCString *insertString, uint64_t index)
{
    if (!string || !insertString || insertString->length == 0 || index > string->length) return;
    
    // Resize our buffer to accommodate the inserted string. If we couldn't resize successfully, return
    uint64_t minimumStringLength = string->length + insertString->length;
    PGCStringReallocateBuffer(string, minimumStringLength);
    if (string->capacity <= minimumStringLength) return;
    
    memmove(&string->buffer[index + insertString->length], &string->buffer[index], (string->length - index) * sizeof(char));
    memmove(&string->buffer[index], &insertString->buffer[0], insertString->length * sizeof(char));
    
    string->length += insertString->length;
    string->buffer[string->length] = '\0';
}


void PGCStringAppendString(PGCString *string, PGCString *appendString)
{
    if (!string) return;
    PGCStringInsertStringAtIndex(string, appendString, string->length);
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