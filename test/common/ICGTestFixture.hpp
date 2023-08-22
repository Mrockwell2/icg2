#include <gtest/gtest.h>
#include <ctype.h>
#include <string>
#include "MemoryManagement/MemoryManager.hpp"

// This function will be pulled in by whichever io_ file the test includes.
// TODO: maybe instead we have the IO file make the whole DataTypeInator? That might be cool.
// We're gonna have to figure out how to work with multiple IO files
extern void populate_type_dictionary(DataTypeInator *);

class ICGTest : public ::testing::Test {
    public:
    DataTypeInator dataTypeInator;
    MemoryManager memoryManager;


    ICGTest () : memoryManager(&dataTypeInator) {
        populate_type_dictionary(&dataTypeInator);
    }

    ~ICGTest () {}

	int strcmp_IgnoringWhiteSpace(const char* s1, const char* s2) {
		int i1 = 0;
		int i2 = 0;

		while (1) {
			while ( !isgraph( s1[i1] ) && s1[i1] != '\0') { i1++; }
			while ( !isgraph( s2[i2] ) && s2[i2] != '\0') { i2++; }
			if ( s1[i1] == '\0' && s2[i2] == '\0') { return 0; }
			if ( s1[i1] != s2[i2]) {
				if (s1[i1] < s2[i2]) {
					return -1;
				} else {
					return 1;
				}
			}
			i1++; i2++;
		}
	}
};