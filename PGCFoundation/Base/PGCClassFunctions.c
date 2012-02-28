//
//  PGCClassFunctions.c
//  PGCFoundation
//
//  Created by Prachi Gauriar on 2/24/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCClassFunctions.h>
#include <PGCFoundation/PGCObject.h>

#pragma mark Creation and Deallocation

PGCClassFunctions *PGCClassFunctionsCreate(PGCCopyFunction *copy, PGCDeallocFunction *dealloc, PGCDescriptionFunction *description, 
                                           PGCEqualsFunction *equals,PGCHashFunction *hash, PGCReleaseFunction *release, PGCRetainFunction *retain)
{
    PGCClassFunctions *functions = calloc(1, sizeof(PGCClassFunctions));
    if (!functions) return NULL;
    
    functions->copy = copy;
    functions->dealloc = dealloc;
    functions->description = description;
    functions->equals = equals;
    functions->hash = hash;
    functions->release = release;
    functions->retain = retain;
    
    return functions;
}


void PGCClassFunctionsDealloc(PGCClassFunctions *functions)
{
    if (functions) free(functions);
}


#pragma mark Convenience Tests

bool PGCClassFunctionsNoneNull(PGCClassFunctions *functions) 
{
    return functions->copy && functions->dealloc && functions->description && functions->equals && 
        functions->hash && functions->release && functions->retain;
}
