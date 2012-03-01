//
//  PGCDecimal.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCDECIMAL_H
#define PGCDECIMAL_H

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCDecimal

typedef struct _PGCDecimal PGCDecimal;

struct _PGCDecimal {
    PGCObject super;
    double value;
};

extern PGCClass *PGCDecimalClass(void);
extern PGCDecimal *PGCDecimalInstanceWithValue(double value);

#pragma mark Basic Functions

extern PGCDecimal *PGCDecimalInitWithValue(PGCDecimal *decimal, double value);
extern PGCType PGCDecimalCopy(PGCType instance);
extern PGCString *PGCDecimalDescription(PGCType instance);
extern bool PGCDecimalEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCDecimalHash(PGCType instance);

#pragma mark Accessors

extern double PGCDecimalGetValue(PGCDecimal *decimal);

#endif
