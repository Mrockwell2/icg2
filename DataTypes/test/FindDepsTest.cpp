#include "Type/EnumDictionary.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Algorithm/FindDeps.hpp"

#include "DataTypeTestSupport.hpp"
#include "Value/PointerValue.hpp"
#include "Value/StringValue.hpp"

#include "gtest/gtest.h"

#include <iostream>

class FindDepsTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    FindDepsTest() {

    }

    ~FindDepsTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

namespace FindDependencies {

TEST_F(FindDepsTest, basic) {
    // ARRANGE
    addPointerTestClassesToDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassWithNestedClasses");

    ClassWithNestedClasses var_to_checkpoint = { .a = { .a = 5, .b = "No Point", .c = { 0 } }, .b = { .a = (void *)(0x12345678) } };

    // ACT
    FindDepsVisitor visitor(&var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResult();
	EXPECT_EQ(3, results.size());
}

}