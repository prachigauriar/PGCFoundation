//
//  PGCCharacter.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCCharacter.h>
#include <PGCFoundation/PGCString.h>

#include <stdio.h>

/*!
 @struct _PGCCharacter
 @abstract PGCCharacter’s corresponding data structure.
 @field super The instance’s superclass’s fields.
 @field value The character value for the instance.
 */
struct _PGCCharacter {
    PGCObject super;
    char value;
};


PGCClass *PGCCharacterClass(void)
{
    static PGCClass *characterClass = NULL;
    if (!characterClass) {
        PGCClassFunctions functions = { PGCCharacterCopy, NULL, PGCCharacterDescription, PGCCharacterEquals, PGCCharacterHash, NULL, NULL };
        characterClass = PGCClassCreate("PGCCharacter", PGCObjectClass(), functions, sizeof(PGCCharacter));
    }
    return characterClass;
}


PGCCharacter *PGCCharacterInstanceWithValue(char value)
{
    return PGCAutorelease(PGCCharacterInitWithValue(NULL, value));
}

#pragma mark Basic Functions

PGCCharacter *PGCCharacterInitWithValue(PGCCharacter *character, char value)
{
    if (!character && (character = PGCAlloc(PGCCharacterClass())) == NULL) return NULL;
    PGCObjectInit(&character->super);
    character->value = value;
    return character;
}


PGCType PGCCharacterCopy(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCCharacterClass())) return NULL;
    return PGCCharacterInitWithValue(NULL, ((PGCCharacter *)instance)->value);    
}


PGCString *PGCCharacterDescription(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCCharacterClass())) return NULL;
    return PGCStringInstanceWithFormat("%c", ((PGCCharacter *)instance)->value);
}


bool PGCCharacterEquals(PGCType instance1, PGCType instance2)
{
    if (!PGCObjectIsKindOfClass(instance1, PGCCharacterClass()) || !PGCObjectIsKindOfClass(instance2, PGCCharacterClass())) return false;
    return ((PGCCharacter *)instance1)->value == ((PGCCharacter *)instance2)->value;
}


uint64_t PGCCharacterHash(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCCharacterClass())) return 0;
    return ((PGCCharacter *)instance)->value;
}


#pragma mark Accessors

char PGCCharacterGetValue(PGCCharacter *character)
{
    return character ? character->value : 0;
}
