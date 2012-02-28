//
//  PGCBoolean.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCBOOLEAN_H
#define PGCBOOLEAN_H

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCBoolean

typedef struct _PGCBoolean PGCBoolean;

struct _PGCBoolean {
    PGCObject super;
    bool value;
};

extern PGCClass *PGCBooleanClass(void);
extern PGCBoolean *PGCBooleanTrue(void);
extern PGCBoolean *PGCBooleanFalse(void);

#pragma mark Basic Functions

extern PGCType PGCBooleanCopy(PGCType instance);
extern PGCString *PGCBooleanDescription(PGCType instance);
extern bool PGCBooleanEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCBooleanHash(PGCType instance);

#pragma mark Accessors

extern bool PGCBooleanGetBoolValue(PGCBoolean *boolean);

#endif
