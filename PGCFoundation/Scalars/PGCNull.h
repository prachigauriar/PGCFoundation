//
//  PGCNull.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCNULL_H
#define PGCNULL_H

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCNull

typedef struct _PGCNull PGCNull;

struct _PGCNull {
    PGCObject super;
};

extern PGCClass *PGCNullClass(void);
extern PGCNull *PGCNullInstance(void);

#pragma mark Basic Functions

extern PGCType PGCNullCopy(PGCType instance);
extern PGCString *PGCNullDescription(PGCType instance);
extern bool PGCNullEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCNullHash(PGCType instance);

#endif
