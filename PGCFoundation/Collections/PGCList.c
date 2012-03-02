//
//  PGCList.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 3/1/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCList.h>

#pragma mark Private Function Interfaces

PGCListNode *PGCListGetNodeAtIndex(PGCList *list, uint64_t index);
void PGCListDealloc(PGCType instance);


#pragma mark - PGCList

PGCClass *PGCListClass(void)
{
    static PGCClass *linkedListClass = NULL;
    if (!linkedListClass) {
        PGCClassFunctions functions = { PGCListCopy, PGCListDealloc, PGCListDescription, PGCListEquals, PGCListHash, NULL, NULL };
        linkedListClass = PGCClassCreate("PGCList", PGCObjectClass(), functions, sizeof(PGCList));
    }
    return linkedListClass;
}


PGCList *PGCListInstance(void)
{
    return PGCAutorelease(PGCListInit(NULL));
}


#pragma mark Basic Functions

PGCList *PGCListInit(PGCList *list)
{
    if (!list && (list = PGCAlloc(PGCListClass())) == NULL) return NULL;
    PGCObjectInit(&list->super);
    list->currentIndex = PGCNotFound;
    return list;
}


void PGCListDealloc(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCListClass())) return;
    PGCListRemoveAllObjects((PGCList *)instance);
    PGCSuperclassDealloc(instance);
}


PGCType PGCListCopy(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCListClass())) return NULL;
    PGCList *list = instance;
    
    PGCList *copy = PGCListInit(NULL);
    for (uint64_t i = 0; i < list->count; i++) PGCListAddObject(copy, PGCListGetObjectAtIndex(list, i));
    
    return copy;
}


PGCString *PGCListDescription(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCListClass())) return NULL;
    PGCString *joinString = PGCListJoinComponentsWithString(instance, PGCStringInstanceWithCString(", "));
    return PGCStringInstanceWithFormat("[%s]", PGCDescriptionCString(joinString));
}


bool PGCListEquals(PGCType instance1, PGCType instance2)
{
    if (!instance1 || !instance2) return false;
    if (!PGCObjectIsKindOfClass(instance1, PGCListClass()) || !PGCObjectIsKindOfClass(instance2, PGCListClass())) return false;
    
    PGCList *list1 = instance1;
    PGCList *list2 = instance2;
    if (list1->count != list2->count) return false;
    
    for (uint64_t i = 0; i < list1->count; i++) {
        PGCType listObject1 = PGCListGetObjectAtIndex(list1, i);
        PGCType listObject2 = PGCListGetObjectAtIndex(list2, i);
        
        if (PGCHash(listObject1) != PGCHash(listObject2)) return false;
        if (!PGCEquals(listObject1, listObject2)) return false;
    }
    
    return true;
}


uint64_t PGCListHash(PGCType instance)
{
    if (!instance || !PGCObjectIsKindOfClass(instance, PGCListClass())) return 0;
    PGCList *list = instance;
    
    // This is the DJB2 hash function, which is a fast, public domain string hash with
    // excellent distribution. It was posted to comp.lang.c by Daniel J. Bernstein.
    // It has been adapted slightly to deal with an list of objects instead of an 
    // array of characters
    uint64_t hash = 5381 + list->count;
    for (uint64_t i = 0; i < list->count; i++) {
        // Set hash to hash * 33 + the current element's value
        hash = ((hash << 5) + hash) + PGCHash(PGCListGetObjectAtIndex(list, i));
    }
    
    return hash;
}


#pragma mark Accessors

uint64_t PGCListGetCount(PGCList *list)
{
    return list ? list->count : 0;
}


bool PGCListContainsObject(PGCList *list, PGCType instance)
{
    return PGCListGetIndexOfObject(list, instance) != PGCNotFound;
}


PGCType PGCListGetObjectAtIndex(PGCList *list, uint64_t index)
{
    PGCListNode *node = PGCListGetNodeAtIndex(list, index);
    return node ? PGCAutorelease(PGCRetain(node->object)) : NULL;
}


PGCListNode *PGCListGetNodeAtIndex(PGCList *list, uint64_t index)
{
    if (!list || index >= list->count) return NULL;

    // Simple optimization: if we already have a pointer to the node we want, don't bother doing any calculating or iterating
    if (index == 0) {
        list->currentIndex = index;
        list->current = list->head;
        return list->current;
    } else if (index == list->count - 1) {
        list->currentIndex = index;
        list->current = list->tail;
        return list->current;
    } else if (index == list->currentIndex) {
        return list->current;
    }
    
    // Otherwise we have some calculating and iterating to do
    uint64_t distanceFromHead = index;
    uint64_t distanceFromTail = list->count - 1 - index;
    uint64_t distanceFromCurrent = (list->currentIndex > index) ? list->currentIndex - index : index - list->currentIndex;
    
    PGCListNode *iterator = NULL;
    uint64_t distanceLeft = 0;
    bool goForward = false;
    
    // Of the head, tail, and current nodes, determine which is closest to the index we want
    if (distanceFromHead <= distanceFromTail && distanceFromHead <= distanceFromCurrent) {
        iterator = list->head;
        distanceLeft = distanceFromHead;
        goForward = true;
    } else if (distanceFromTail <= distanceFromHead && distanceFromTail <= distanceFromCurrent) {
        iterator = list->tail;
        distanceLeft = distanceFromTail;
        goForward = false;
    } else {
        iterator = list->current;
        distanceLeft = distanceFromCurrent;
        goForward = (index > list->currentIndex);
    }
    
    while (distanceLeft > 0) {
        iterator = goForward ? iterator->next : iterator->previous;
        distanceLeft--;
    }
    
    list->currentIndex = index;
    list->current = iterator;

    return list->current;
}


PGCType PGCListGetFirstObject(PGCList *list)
{
    return PGCListGetObjectAtIndex(list, 0);
}


PGCType PGCListGetLastObject(PGCList *list)
{
    return list && list->count > 0 ? PGCListGetObjectAtIndex(list, list->count - 1) : NULL;
}


uint64_t PGCListGetIndexOfObject(PGCList *list, PGCType instance)
{
    if (!list || !instance || list->count == 0) return PGCNotFound;
    
    uint64_t instanceHash = PGCHash(instance);
    for (uint64_t i = 0; i < list->count; i++) {
        PGCType object = PGCListGetObjectAtIndex(list, i);
        if (PGCHash(object) == instanceHash && PGCEquals(instance, object)) return i;
    }
    
    return PGCNotFound;
}


uint64_t PGCListGetIndexOfIdenticalObject(PGCList *list, PGCType instance)
{
    if (!list || !instance || list->count == 0) return PGCNotFound;
    for (uint64_t i = 0; i < list->count; i++) if (PGCListGetObjectAtIndex(list, i) == instance) return i;
    return PGCNotFound;
}


#pragma mark Object Addition, Replacement, and Removal

void PGCListAddObject(PGCList *list, PGCType instance)
{
    PGCListInsertObjectAtIndex(list, instance, list->count);
}


void PGCListInsertObjectAtIndex(PGCList *list, PGCType instance, uint64_t index)
{
    if (!list || index > list->count) return;
    
    PGCListNode *newNode = calloc(1, sizeof(PGCListNode));
    if (!newNode) return;
    newNode->object = PGCRetain(instance);
    
    if (index == 0) {
        if (list->head) {
            newNode->next = list->head;
            list->head->previous = newNode;
        }
        
        list->head = newNode;
    } else {
        PGCListNode *previousNode = PGCListGetNodeAtIndex(list, index - 1);
        if (previousNode->next) {
            newNode->next = previousNode->next;
            newNode->next->previous = newNode;
        }
        
        previousNode->next = newNode;
        newNode->previous = previousNode;
    }
    
    if (index == list->count) {
        list->tail = newNode;
    }
    
    list->current = newNode;
    list->currentIndex = index;
    list->count++;
}


void PGCListPushObject(PGCList *list, PGCType instance)
{
    PGCListInsertObjectAtIndex(list, instance, 0);    
}


void PGCListExchangeValuesAtIndices(PGCList *list, uint64_t index1, uint64_t index2)
{
    // Note that we cannot optimize for when index1 == index2 because we need to move the list’s current pointer
    if (!list || index1 >= list->count || index2 >= list->count) return;

    PGCListNode *node1 = PGCListGetNodeAtIndex(list, index1);
    PGCListNode *node2 = PGCListGetNodeAtIndex(list, index2);
    
    PGCType node1Object = node1->object;
    node1->object = node2->object;
    node2->object = node1Object;
}


void PGCListReplaceObjectAtIndex(PGCList *list, PGCType instance, uint64_t index)
{
    if (!list || index >= list->count) return;
    PGCListNode *node = PGCListGetNodeAtIndex(list, index);
    PGCRelease(node->object);
    node->object = PGCRetain(instance);
}


PGCType PGCListPopObject(PGCList *list)
{
    if (!list || list->count == 0) return NULL;
    PGCType object = PGCListGetObjectAtIndex(list, 0);
    PGCListRemoveObjectAtIndex(list, 0);
    return object;
}


void PGCListRemoveObject(PGCList *list, PGCType instance)
{
    uint64_t indexOfObject; 
    while ((indexOfObject = PGCListGetIndexOfObject(list, instance)) != PGCNotFound) PGCListRemoveObjectAtIndex(list, indexOfObject);
}


void PGCListRemoveObjectAtIndex(PGCList *list, uint64_t index)
{
    if (!list || index >= list->count) return;
    
    PGCListNode *oldNode = NULL;
    
    if (index == 0) {
        oldNode = list->head;
        if (oldNode->next) oldNode->next->previous = NULL;

        list->head = list->head->next;
        list->current = list->head;
        list->currentIndex = 0;
    } else {
        PGCListNode *previousNode = PGCListGetNodeAtIndex(list, index - 1);
        oldNode = previousNode->next;
        if (oldNode->next) oldNode->next->previous = previousNode;
        previousNode->next = oldNode->next;
        
        // No need to update current node information in list, as that is handled in PGCListGetNodeAtIndex
    }
    
    if (index == list->count - 1) list->tail = oldNode->previous;    
    if (--list->count == 0) list->currentIndex = PGCNotFound;
    
    PGCRelease(oldNode->object);
    free(oldNode);
}


void PGCListRemoveAllObjects(PGCList *list)
{
    PGCListNode *node = list->head;
    while (node) {
        PGCListNode *next = node->next;
        PGCRelease(node->object);
        free(node);
        node = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    list->count = 0;
    list->currentIndex = PGCNotFound;
}


#pragma mark String Conversion

PGCString *PGCListJoinComponentsWithString(PGCList *list, PGCString *separator)
{
    if (!list) return NULL;
    
    PGCString *join = PGCStringInstance();
    for (uint64_t i = 0; i < list->count; i++) {
        PGCStringAppendString(join, PGCDescription(PGCListGetObjectAtIndex(list, i)));
        if (i != list->count - 1) PGCStringAppendString(join, separator);
    }
    
    return join;
}
