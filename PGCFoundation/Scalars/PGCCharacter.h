//
//  PGCCharacter.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCCHARACTER_H
#define PGCCHARACTER_H

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCCharacter

typedef struct _PGCCharacter PGCCharacter;

struct _PGCCharacter {
    PGCObject super;
    char value;
};

extern PGCClass *PGCCharacterClass(void);
extern PGCCharacter *PGCharacterInstanceWithValue(char value);

#pragma mark Basic Functions
extern PGCCharacter *PGCCharacterInitWithValue(PGCCharacter *character, char value);
extern PGCType PGCCharacterCopy(PGCType instance);
extern PGCString *PGCCharacterDescription(PGCType instance);
extern bool PGCCharacterEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCCharacterHash(PGCType instance);

#pragma mark Accessors

extern char PGCCharacterGetValue(PGCCharacter *character);

#endif
