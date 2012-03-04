//
//  PGCList.h
//  PGCFoundation
//
//  Created by Prachi Gauriar on 3/1/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#ifndef PGCLIST_H
#define PGCLIST_H

#include <PGCFoundation/PGCObject.h>
#include <PGCFoundation/PGCString.h>

#pragma mark - PGCList

typedef struct _PGCList PGCList;

extern PGCClass *PGCListClass(void);
extern PGCList *PGCListInstance(void);
extern PGCList *PGCListWithObjects(PGCType object1, ...);

#pragma mark Basic Functions

extern PGCList *PGCListInit(PGCList *list);
extern PGCList *PGCListInitWithObjects(PGCList *list, PGCType object1, ...);

extern PGCType PGCListCopy(PGCType instance);
extern PGCString *PGCListDescription(PGCType instance);
extern bool PGCListEquals(PGCType instance1, PGCType instance2);
extern uint64_t PGCListHash(PGCType instance);

#pragma mark Accessors

extern uint64_t PGCListGetCount(PGCList *list);
extern bool PGCListContainsObject(PGCList *list, PGCType instance);

extern PGCType PGCListGetObjectAtIndex(PGCList *list, uint64_t index);
extern PGCType PGCListGetFirstObject(PGCList *list);
extern PGCType PGCListGetLastObject(PGCList *list);

extern uint64_t PGCListGetIndexOfObject(PGCList *list, PGCType instance);
extern uint64_t PGCListGetIndexOfObjectInRange(PGCList *list, PGCType instance, PGCRange range);
extern uint64_t PGCListGetIndexOfIdenticalObject(PGCList *list, PGCType instance);
extern uint64_t PGCListGetIndexOfIdenticalObjectInRange(PGCList *list, PGCType instance, PGCRange range);

#pragma mark Sublists

extern PGCList *PGCListSublistWithRange(PGCList *list, PGCRange range);

#pragma mark Object Addition, Replacement, and Removal

extern void PGCListAddObject(PGCList *list, PGCType instance);
extern void PGCListInsertObjectAtIndex(PGCList *list, PGCType instance, uint64_t index);
extern void PGCListPushObject(PGCList *list, PGCType instance);

extern void PGCListExchangeValuesAtIndices(PGCList *list, uint64_t index1, uint64_t index2);

extern void PGCListReplaceObjectAtIndex(PGCList *list, PGCType instance, uint64_t index);

extern PGCType PGCListPopObject(PGCList *list);
extern void PGCListRemoveObject(PGCList *list, PGCType instance);
extern void PGCListRemoveObjectAtIndex(PGCList *list, uint64_t index);
extern void PGCListRemoveAllObjects(PGCList *list);

#pragma mark String Conversion

extern PGCString *PGCListJoinComponentsWithString(PGCList *list, PGCString *separator);

#endif
