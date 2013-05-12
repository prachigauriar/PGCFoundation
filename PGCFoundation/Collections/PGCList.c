//
//  PGCList.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 3/1/2012.
//  Copyright (c) 2012 Prachi Gauriar.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <PGCFoundation/PGCList.h>

#pragma mark - PGCListNode

typedef struct _PGCListNode PGCListNode;
struct _PGCListNode {
    PGCListNode *previous;
    PGCListNode *next;
    PGCType object;
};


struct _PGCList {
    PGCObject super;
    
    PGCListNode *head;
    PGCListNode *tail;
    PGCListNode *current;
    
    uint64_t count;
    uint64_t currentIndex;
};

#pragma mark Private Function Interfaces

PGCList *PGCListInitWithObjectAndObjectArguments(PGCList *list, PGCType firstObject, va_list objectArguments);
void PGCListDealloc(PGCType instance);
PGCListNode *PGCListGetNodeAtIndex(PGCList *list, uint64_t index);


#pragma mark - PGCList

PGCClass *PGCListClass(void)
{
    static PGCClass *listClass = NULL;
    if (!listClass) {
        PGCClassFunctions functions = { PGCListCopy, PGCListDealloc, PGCListDescription, PGCListEquals, PGCListHash, NULL, NULL };
        listClass = PGCClassCreate("PGCList", PGCObjectClass(), functions, sizeof(PGCList));
    }
    return listClass;
}


PGCList *PGCListInstance(void)
{
    return PGCAutorelease(PGCListInit(NULL));
}


PGCList *PGCListInstanceWithObjects(PGCType object1, ...)
{
    va_list arguments;
    
    va_start(arguments, object1);
    PGCList *list = PGCListInitWithObjectAndObjectArguments(NULL, object1, arguments);    
    va_end(arguments);
    
    return PGCAutorelease(list);
}


#pragma mark Basic Functions

PGCList *PGCListInit(PGCList *list)
{
    if (!list && (list = PGCAlloc(PGCListClass())) == NULL) return NULL;
    PGCObjectInit(&list->super);
    list->currentIndex = PGCNotFound;
    return list;
}


PGCList *PGCListInitWithObjects(PGCList *list, PGCType object1, ...)
{
    va_list arguments;
    
    va_start(arguments, object1);
    list = PGCListInitWithObjectAndObjectArguments(list, object1, arguments);    
    va_end(arguments);
    
    return list;
}


PGCList *PGCListInitWithObjectAndObjectArguments(PGCList *list, PGCType firstObject, va_list objectArguments)
{
    list = PGCListInit(list);
    if (!list) return NULL;
    
    PGCType object = firstObject;
    while (object) {
        PGCListAddObject(list, object);   
        object = va_arg(objectArguments, PGCType);
    }
    
    return list;
}


void PGCListDealloc(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCListClass())) return;
    PGCListRemoveAllObjects((PGCList *)instance);
    PGCSuperclassDealloc(instance);
}


PGCType PGCListCopy(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCListClass())) return NULL;
    PGCList *list = instance;
    
    PGCList *copy = PGCListInit(NULL);
    for (uint64_t i = 0; i < list->count; i++) PGCListAddObject(copy, PGCListGetNodeAtIndex(list, i)->object);
    
    return copy;
}


PGCString *PGCListDescription(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCListClass())) return NULL;
    PGCString *joinString = PGCListJoinComponentsWithString(instance, PGCStringInstanceWithCString(", "));
    return PGCStringInstanceWithFormat("[%s]", PGCDescriptionCString(joinString));
}


bool PGCListEquals(PGCType instance1, PGCType instance2)
{
    if (!PGCObjectIsKindOfClass(instance1, PGCListClass()) || !PGCObjectIsKindOfClass(instance2, PGCListClass())) return false;
    
    PGCList *list1 = instance1;
    PGCList *list2 = instance2;
    if (list1->count != list2->count) return false;
    
    for (uint64_t i = 0; i < list1->count; i++) {
        PGCType listObject1 = PGCListGetNodeAtIndex(list1, i)->object;
        PGCType listObject2 = PGCListGetNodeAtIndex(list2, i)->object;
        
        if (PGCHash(listObject1) != PGCHash(listObject2)) return false;
        if (!PGCEquals(listObject1, listObject2)) return false;
    }
    
    return true;
}


uint64_t PGCListHash(PGCType instance)
{
    if (!PGCObjectIsKindOfClass(instance, PGCListClass())) return 0;
    return ((PGCList *)instance)->count;
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
    
    while (distanceLeft-- > 0) {
        iterator = goForward ? iterator->next : iterator->previous;
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
    return list ? PGCListGetIndexOfObjectInRange(list, instance, PGCMakeRange(0, list->count)) : PGCNotFound;
}


uint64_t PGCListGetIndexOfObjectInRange(PGCList *list, PGCType instance, PGCRange range)
{
    if (!list || !instance || list->count == 0 || range.location >= list->count || range.location + range.length > list->count) return PGCNotFound;
    
    uint64_t instanceHash = PGCHash(instance);
    
    uint64_t lastIndex = range.location + range.length;
    for (uint64_t i = range.location; i < lastIndex; i++) {
        PGCType object = PGCListGetNodeAtIndex(list, i)->object;
        if (PGCHash(object) == instanceHash && PGCEquals(instance, object)) return i;
    }
    
    return PGCNotFound;
}

uint64_t PGCListGetIndexOfIdenticalObject(PGCList *list, PGCType instance)
{
    return list ? PGCListGetIndexOfIdenticalObjectInRange(list, instance, PGCMakeRange(0, list->count)) : PGCNotFound;
}


uint64_t PGCListGetIndexOfIdenticalObjectInRange(PGCList *list, PGCType instance, PGCRange range)
{
    if (!list || !instance || list->count == 0 || range.location >= list->count || range.location + range.length > list->count) return PGCNotFound;
    
    uint64_t lastIndex = range.location + range.length;
    for (uint64_t i = range.location; i < lastIndex; i++) {
        if (PGCListGetNodeAtIndex(list, i)->object == instance) return i;   
    }
    
    return PGCNotFound;
}


#pragma mark Sublists

PGCList *PGCListSublistWithRange(PGCList *list, PGCRange range)
{
    if (!list || range.location >= list->count || range.location + range.length > list->count) return NULL;
    
    PGCList *sublist = PGCListInit(NULL);
    uint64_t lastIndex = range.location + range.length;
    for (uint64_t i = range.location; i < lastIndex; i++) {
        PGCListAddObject(sublist, PGCListGetNodeAtIndex(list, i)->object);
    }
    
    return PGCAutorelease(sublist);
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
    PGCAutoreleasePool *pool = PGCAutoreleasePoolCreate();

    PGCString *join = PGCStringInit(NULL);
    for (uint64_t i = 0; i < list->count; i++) {
        PGCStringAppendString(join, PGCDescription(PGCListGetNodeAtIndex(list, i)->object));
        if (i != list->count - 1) PGCStringAppendString(join, separator);
    }
    
    PGCAutoreleasePoolDestroy(pool);
    return PGCAutorelease(join);
}
