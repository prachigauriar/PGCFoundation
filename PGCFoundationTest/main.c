//
//  main.c
//  PGCFoundationTest
//
//  Created by Prachi Gauriar on 2/24/2012.
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

#include <PGCFoundation/PGCFoundation.h>
#include <stdio.h>
#include <string.h>

void TestArrays(void);
void TestDictionaries(void);
void TestStrings(void);

void GenerateGroups(const char *filename, uint64_t groupCount);
char *GetLineFromFile(FILE *file);

int main(int argc, const char * argv[])
{
    PGCAutoreleasePool *pool = PGCAutoreleasePoolCreate();

    printf("Testing arrays...");
    TestArrays();
    
    printf("\nTesting dictionaries...\n");
    TestDictionaries();

    printf("\nTesting strings...\n");
    TestStrings();

    printf("\nGenerating groups...\n");
    GenerateGroups("/Users/prachi/Developer/Ruby/GroupCreator/FullClass", 7);

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
    PGCString *string = PGCStringInitWithCString(NULL, "aBcdEf");
    PGCString *cString = PGCStringInstanceWithCString("123456");

    printf("Originally, string = \"%s\"\n", PGCDescriptionCString(string));
    
    PGCStringPrependString(string, cString);
    printf("After prepend, string = \"%s\"\n", PGCDescriptionCString(string));
    
    PGCStringAppendString(string, cString);
    printf("After append, string = \"%s\"\n", PGCDescriptionCString(string));
    
    PGCStringInsertStringAtIndex(string, cString, 6);
    printf("After insert, string = \"%s\"\n", PGCDescriptionCString(string));

    PGCStringReplaceCharactersInRangeWithString(string, PGCMakeRange(3, 12), PGCStringInstance());
    printf("After replace, string = \"%s\"\n", PGCDescriptionCString(string));

    printf("Substring to 9: \"%s\"\n", PGCDescriptionCString(PGCStringGetSubstringToIndex(string, 9)));
    printf("Substring in { 3, 6 }: \"%s\"\n", PGCDescriptionCString(PGCStringGetSubstringWithRange(string, PGCMakeRange(3, 6))));
    printf("Substring from 9: \"%s\"\n", PGCDescriptionCString(PGCStringGetSubstringFromIndex(string, 9)));
    
    printf("Uppercase string: \"%s\"\n", PGCDescriptionCString(PGCStringGetUppercaseString(string)));
    printf("Lowercase string: \"%s\"\n", PGCDescriptionCString(PGCStringGetLowercaseString(string)));

    PGCRelease(string);
}


void GenerateGroups(const char *filename, uint64_t groupCount)
{
    FILE *groupsFile = fopen(filename, "r");
    if (!groupsFile) return;
    
    PGCArray *names = PGCArrayInstance();
    
    char *line = NULL;
    while ((line = GetLineFromFile(groupsFile))) {
        PGCArrayAddObject(names, PGCStringInstanceWithCString(line));
        free(line);
    }

    fclose(groupsFile);
    
    srandomdev();
    
    uint64_t nameCount = PGCArrayGetCount(names);
    PGCArray *groups = PGCArrayInitWithInitialCapacity(NULL, groupCount);
    for (uint64_t i = 0; i < groupCount; i++) {
        PGCArrayAddObject(groups, PGCArrayInstance());
    }
    
    uint64_t assignedNameCount = 0;
    while (assignedNameCount < nameCount) {
        PGCString *name = PGCArrayGetObjectAtIndex(names, random() % PGCArrayGetCount(names));
        PGCArrayAddObject(PGCArrayGetObjectAtIndex(groups, assignedNameCount % groupCount), name);
        PGCArrayRemoveObjectAtIndex(names, random() % PGCArrayGetCount(names));
        ++assignedNameCount;
    }

    for (uint64_t i = 0; i < groupCount; ++i) {
        printf("Group %llu:\n", i + 1);
        
        PGCArray *group = PGCArrayGetObjectAtIndex(groups, i);
        uint64_t groupMemberCount = PGCArrayGetCount(group);
        for (uint64_t j = 0; j < groupMemberCount; ++j) {
            printf("\t%s\n", PGCDescriptionCString(PGCArrayGetObjectAtIndex(group, j)));
        }
    }
    
    PGCRelease(groups);
}

char *GetLineFromFile(FILE *file)
{
    if (!file || feof(file)) return NULL;
    
    size_t maxLength = 256;
    size_t length = 0;
    
    char *line = malloc((maxLength + 1) * sizeof(char));
    if (!line) return NULL;
    
    int characterRead = '\0';
    
    while ((characterRead = getc(file)) != EOF && characterRead != '\n') {
        if (length >= maxLength) {
            maxLength *= 2;
            
            char *reallocedLine = realloc(line, (maxLength + 1) * sizeof(char));
            if (!reallocedLine) {
                free(line);
                return NULL;
            }
            
            line = reallocedLine;
        }
        
        line[length++] = (char)characterRead;
    }
    
    if (ferror(file)) {
        free(line);
        return NULL;
    }
    
    /* Try to reduce the size of the line to exactly fit line's length */
    line[length] = '\0';
    char *reallocedLine = realloc(line, (length + 1) * sizeof(char));
    if (reallocedLine) line = reallocedLine;
    
    return line;
}

