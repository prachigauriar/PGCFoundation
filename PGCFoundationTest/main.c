//
//  main.c
//  PGCFoundationTest
//
//  Created by Prachi Gauriar on 2/24/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCFoundation.h>
#include <stdio.h>

int main(int argc, const char * argv[])
{
    PGCAutoreleasePool *pool = PGCAutoreleasePoolCreate();

    PGCArray *array = PGCArrayNew();
    for (uint64_t i = 0; i < 32; i++) {
        PGCArrayPushObject(array, PGCIntegerNewWithUnsignedValue(i));
    }
    
    PGCArrayInsertObjectAtIndex(array, PGCNullInstance(), 1);
    
    printf("array = %s\n", PGCDescriptionCString(array));
    
    PGCType object = NULL;
    while ((object = PGCArrayPopObject(array))) {
        printf("POP array => %p (%s)\n", object, PGCDescriptionCString(object));
    }
    
    PGCAutoreleasePoolDestroy(pool);
    pool = PGCAutoreleasePoolCreate();
    
    PGCString *string = PGCStringInitWithCString(NULL, "123456");
    PGCString *cString = PGCStringNewWithCString("abc");
    
    printf("\nOriginally, string = \"%s\"\n", PGCDescriptionCString(string));

    PGCStringPrependString(string, cString);
    printf("After prepend, string = \"%s\"\n", PGCDescriptionCString(string));
    
    PGCStringAppendString(string, cString);
    printf("After append, string = \"%s\"\n", PGCDescriptionCString(string));

    PGCStringInsertStringAtIndex(string, cString, 6);
    printf("After insert, string = \"%s\"\n", PGCDescriptionCString(string));

    PGCRelease(string);
    
    PGCAutoreleasePoolDestroy(pool);
    return 0;
}
