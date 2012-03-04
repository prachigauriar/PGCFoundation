//
//  main.c
//  PGCFoundationTest
//
//  Created by Prachi Gauriar on 2/24/2012.
//  Copyright (c) 2012 Prachi Gauriar. All rights reserved.
//

#include <PGCFoundation/PGCFoundation.h>
#include <stdio.h>

void TestArrays(void);
void TestDictionaries(void);
void TestStrings(void);


int main(int argc, const char * argv[])
{
    PGCAutoreleasePool *pool = PGCAutoreleasePoolCreate();

    printf("Testing arrays...");
    TestArrays();
    
    printf("\nTesting dictionaries...\n");
    TestDictionaries();

    
    printf("\nTesting strings...\n");
    TestStrings();
        
    PGCAutoreleasePoolDestroy(pool);
    return 0;
}


void TestArrays(void)
{
    PGCArray *array = PGCArrayInstance();
    for (uint64_t i = 1; i <= 3; i++) {
        for (uint64_t j = 0; j < 6; j++) {
            PGCType object = NULL;
        
            switch (j % 6) {
                case 0:
                    object = i % 2 ? PGCBooleanTrue() : PGCBooleanFalse();
                    break;
                case 1:
                    object = PGCCharacterInstanceWithValue('A' + i * j);
                    break;
                case 2:
                    object = PGCDecimalInstanceWithValue(0.123 * i);
                    break;
                case 3:
                    object = PGCNullInstance();
                    break;
                case 4:
                    object = i % 2 ? PGCIntegerInstanceWithSignedValue(-1 * i * j) : PGCIntegerInstanceWithUnsignedValue(i * j);
                    break;
                case 5:
                    object = PGCStringInstanceWithFormat("\"%d\"", i * j);
                    break;
            }
            
            PGCArrayAddObject(array, object);
        }
    }
        
    printf("array = %s\n", PGCDescriptionCString(array));
    
    PGCType object = NULL;
    while ((object = PGCArrayPopObject(array))) {
        printf("POP array => %p (%s)\n", object, PGCDescriptionCString(object));
    }

}


void TestDictionaries(void)
{
    PGCDictionary *dictionary = PGCDictionaryInstance();
    
    for (uint64_t i = 1; i <= 3; i++) {
        for (uint64_t j = 0; j < 6; j++) {
            PGCType object = NULL;
            switch (j % 6) {
                case 0:
                    object = i % 2 ? PGCBooleanTrue() : PGCBooleanFalse();
                    break;
                case 1:
                    object = PGCCharacterInstanceWithValue('A' + i * j);
                    break;
                case 2:
                    object = PGCDecimalInstanceWithValue(0.123 * i);
                    break;
                case 3:
                    object = PGCNullInstance();
                    break;
                case 4:
                    object = i % 2 ? PGCIntegerInstanceWithSignedValue(-1 * i * j) : PGCIntegerInstanceWithUnsignedValue(i * j);
                    break;
                case 5:
                    object = PGCStringInstanceWithFormat("\"%llu\"", i * j);
                    break;
            }

            PGCString *key = PGCStringInstanceWithFormat("Key %llu", 6 * (i - 1) + j);
            printf("Setting %s => %s\n", PGCDescriptionCString(key), PGCDescriptionCString(object));
            PGCDictionarySetObjectForKey(dictionary, object, key);
        }
    }
    
    uint64_t valueCount = PGCDictionaryGetCount(dictionary);
    for (uint64_t i = 0; i < valueCount; i++) {
        PGCString *key = PGCStringInstanceWithFormat("Key %llu", i);
        PGCString *object = PGCDictionaryGetObjectForKey(dictionary, key);
        printf("%s => %s\n", PGCDescriptionCString(key), PGCDescriptionCString(object));
    }

    PGCType key = PGCStringInstanceWithCString("Key 7");
    PGCType oldValue = PGCDictionaryGetObjectForKey(dictionary, key);
    PGCType newValue = PGCStringInstanceWithCString("DIFFERENT THAN IT WAS");
    printf("Resetting %s. Did map to %s. Will map to %s\n", PGCDescriptionCString(key), PGCDescriptionCString(oldValue), PGCDescriptionCString(newValue));
    PGCDictionarySetObjectForKey(dictionary, newValue, key);
    PGCType object = PGCDictionaryGetObjectForKey(dictionary, key);
    printf("%s => %s\n", PGCDescriptionCString(key), PGCDescriptionCString(object));
    
    
    PGCArray *allKeys = PGCDictionaryGetAllKeys(dictionary);
    while ((key = PGCArrayPopObject(allKeys))) {
        PGCDictionaryRemoveObjectForKey(dictionary, key);
        if (PGCArrayGetCount(allKeys) != PGCDictionaryGetCount(dictionary)) {
            printf("Dictionary count (%llu) does not match array count (%llu)\n", PGCDictionaryGetCount(dictionary), PGCArrayGetCount(allKeys));
        }
    }
}


void TestStrings(void)
{    
    PGCString *string = PGCStringInitWithCString(NULL, "abc123");
    PGCString *cString = PGCStringInstanceWithCString("xyz890");
    
    printf("Originally, string = \"%s\"\n", PGCDescriptionCString(string));
    
    PGCStringPrependString(string, cString);
    printf("After prepend, string = \"%s\"\n", PGCDescriptionCString(string));
    
    PGCStringAppendString(string, cString);
    printf("After append, string = \"%s\"\n", PGCDescriptionCString(string));
    
    PGCStringInsertStringAtIndex(string, cString, 6);
    printf("After insert, string = \"%s\"\n", PGCDescriptionCString(string));
    
    printf("Substring to 8: \"%s\"\n", PGCDescriptionCString(PGCStringGetSubstringToIndex(string, 8)));
    printf("Substring in { 3, 9 }: \"%s\"\n", PGCDescriptionCString(PGCStringGetSubstringWithRange(string, PGCMakeRange(3, 9 ))));
    printf("Substring from 8: \"%s\"\n", PGCDescriptionCString(PGCStringGetSubstringFromIndex(string, 8)));
    
    printf("Uppercase string: \"%s\"\n", PGCDescriptionCString(PGCStringGetUppercaseString(string)));
    printf("Lowercase string: \"%s\"\n", PGCDescriptionCString(PGCStringGetLowercaseString(string)));

    PGCRelease(string);
}