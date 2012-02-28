//
//  PGCObject.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/22/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCOBJECT_H
#define PGCOBJECT_H

#include <PGCFoundation/PGCBase.h>
#include <PGCFoundation/PGCClass.h>

#pragma mark - PGCObject 

typedef struct _PGCObject PGCObject;

struct _PGCObject {
    PGCClass *class;
    uint64_t retainCount;
};

extern PGCClass *PGCObjectClass();

#pragma mark Basic Functions

extern PGCObject *PGCObjectInitWithClass(PGCObject *object, PGCClass *class);
extern PGCString *PGCObjectDescription(PGCType instance);
extern bool PGCObjectEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCObjectHash(PGCType instance);
extern void PGCObjectRelease(PGCType instance);
extern PGCType PGCObjectRetain(PGCType instance);

#pragma mark Class Introspection

extern PGCClass *PGCObjectGetClass(PGCType instance);
extern bool PGCObjectIsKindOfClass(PGCType instance, PGCClass *class);

#pragma mark Polymorphic Functions

extern PGCType PGCCopy(PGCType instance);
extern void PGCDealloc(PGCType instance);
extern PGCString *PGCDescription(PGCType instance);
extern const char *PGCDescriptionCString(PGCType instance);
extern bool PGCEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCHash(PGCType instance);
extern void PGCRelease(PGCType instance);
extern PGCType PGCRetain(PGCType instance);

extern PGCType PGCAutorelease(PGCType instance);


extern PGCType PGCSuperclassCopy(PGCType instance);
extern void PGCSuperclassDealloc(PGCType instance);
extern PGCString *PGCSuperclassDescription(PGCType instance);
extern bool PGCSuperclassEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCSuperclassHash(PGCType instance);
extern void PGCSuperclassRelease(PGCType instance);
extern PGCType PGCSuperclassRetain(PGCType instance);

#pragma mark Convenience Accessors for Class Functions

extern PGCCopyFunction *PGCObjectGetCopyFunction(PGCType instance);
extern PGCDeallocFunction *PGCObjectGetDeallocFunction(PGCType instance);
extern PGCDescriptionFunction *PGCObjectGetDescriptionFunction(PGCType instance);
extern PGCEqualsFunction *PGCObjectGetEqualsFunction(PGCType instance);
extern PGCHashFunction *PGCObjectGetHashFunction(PGCType instance);
extern PGCReleaseFunction *PGCObjectGetReleaseFunction(PGCType instance);
extern PGCRetainFunction *PGCObjectGetRetainFunction(PGCType instance);

#endif
