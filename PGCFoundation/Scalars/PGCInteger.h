//
//  PGCInteger.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/23/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCINTEGER_H
#define PGCINTEGER_H

#include <PGCFoundation/PGCObject.h>

#pragma mark - PGCInteger

typedef struct _PGCInteger PGCInteger;

struct _PGCInteger {
    PGCObject super;
    bool isSigned;
    union _PGCIntegerValue {
        int64_t signedValue;
        uint64_t unsignedValue;        
    } value;
};

extern PGCClass *PGCIntegerClass(void);
extern PGCInteger *PGCIntegerInstanceWithSignedValue(int64_t value);
extern PGCInteger *PGCIntegerInstanceWithUnsignedValue(uint64_t value);

#pragma mark Basic Functions

extern PGCInteger *PGCIntegerInitWithSignedValue(PGCInteger *integer, int64_t value);
extern PGCInteger *PGCIntegerInitWithUnsignedValue(PGCInteger *integer, uint64_t value);
extern PGCType PGCIntegerCopy(PGCType instance);
extern PGCString *PGCIntegerDescription(PGCType instance);
extern bool PGCIntegerEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCIntegerHash(PGCType instance);

#pragma mark Accessors

extern bool PGCIntegerIsSigned(PGCInteger *integer);
extern bool PGCIntegerIsUnsigned(PGCInteger *integer);

extern int64_t PGCIntegerGetSignedValue(PGCInteger *integer);
extern uint64_t PGCIntegerGetUnsignedValue(PGCInteger *integer);

#endif
