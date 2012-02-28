//
//  PGCAutoreleasePool.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/26/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCAUTORELEASEPOOL_H
#define PGCAUTORELEASEPOOL_H

#include <PGCFoundation/PGCBase.h>

typedef struct _PGCAutoreleasePool PGCAutoreleasePool;

// FIXME: Add logging
PGCAutoreleasePool *PGCAutoreleasePoolCreate(void);
void PGCAutoreleasePoolAddObject(PGCType instance);
void PGCAutoreleasePoolDestroy(PGCAutoreleasePool *pool);

#endif
