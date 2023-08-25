#include "Type/EnumDictionary.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Type/SpecifiedSequenceDataType.hpp"

#include "Algorithm/FindLeaves.hpp"

#include "DataTypeTestSupport.hpp"
#include "Value/PointerValue.hpp"
#include "Value/StringValue.hpp"

#include "gtest/gtest.h"

class FindLeavesTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    FindLeavesTest() {

    }

    ~FindLeavesTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

void verifyIntValue (const FindLeaves::Leaf& leaf, std::string expected_name, int expected_value) {
    EXPECT_EQ(expected_name, leaf.name_stack.toString());

    IntegerValue * int_val = dynamic_cast <IntegerValue *> (leaf.value);
    ASSERT_TRUE(int_val != NULL);
    ASSERT_EQ (expected_value, int_val->getIntegerValue());
}

void verifyDoubleValue (const FindLeaves::Leaf& leaf, std::string expected_name, double expected_value) {
    EXPECT_EQ(expected_name, leaf.name_stack.toString());

    FloatingPointValue * float_val = dynamic_cast <FloatingPointValue *> (leaf.value);
    ASSERT_TRUE(float_val != NULL);
    ASSERT_EQ (expected_value, float_val->getFloatingPointValue());
}

namespace FindLeaves {

TEST_F(FindLeavesTest, basic) {
    // ARRANGE
    std::shared_ptr<DataType> int_data_type (new IntDataType);
    int var_to_checkpoint = 100;

    // ACT
    FindLeavesVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(int_data_type);

    // ASSERT
    auto results = visitor.getResult();
    ASSERT_EQ(results.size(), 1);
    verifyIntValue(results[0], "var_to_checkpoint", var_to_checkpoint);
}

TEST_F(FindLeavesTest, array) {
    // ARRANGE
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("int[5]");
    int var_to_checkpoint[5] = {1, 2, 3, 4, 5};

    // ACT
    FindLeavesVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResult();
    ASSERT_EQ(results.size(), 5);
    for (int i = 0; i < 5; i++) {
        std::string var_name = "var_to_checkpoint[" + std::to_string(i) + "]";
        verifyIntValue(results[i], var_name, var_to_checkpoint[i]);
    }
}

TEST_F(FindLeavesTest, vector) {
    // ARRANGE
    std::vector<int> var_to_checkpoint = {1, 2, 3, 4, 5};
    std::shared_ptr<DataType> data_type (new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));
    data_type->validate(&dataTypeInator);

    // ACT
    FindLeavesVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResult();
    ASSERT_EQ(results.size(), 6);
    
    EXPECT_EQ(true, results[0].is_stl);
    EXPECT_EQ(var_to_checkpoint.size(), results[0].stl_size);
    EXPECT_EQ(std::string("var_to_checkpoint.size"), results[0].name_stack.toString());

    for (int i = 1; i < results.size(); i++) {
        std::string var_name = "var_to_checkpoint[" + std::to_string(i-1) + "]";
        verifyIntValue(results[i], var_name, var_to_checkpoint[i-1]);
    }
}

TEST_F(FindLeavesTest, composite1) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassOne");
    ClassOne var_to_checkpoint = {.a = 5, .b = 1.5};

    // ACT
    FindLeavesVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResult();
    ASSERT_EQ(results.size(), 2);
    verifyIntValue(results[0], "var_to_checkpoint.a", var_to_checkpoint.a);
    verifyDoubleValue(results[1], "var_to_checkpoint.b", var_to_checkpoint.b);
}

TEST_F(FindLeavesTest, composite2) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassTwo");
    ClassTwo var_to_checkpoint = {.x = 100, .y = 5.5, .c1 = {.a = 5, .b = 1.5}};

    // ACT
    FindLeavesVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResult();
    ASSERT_EQ(results.size(), 4);
    verifyIntValue(results[0], "var_to_checkpoint.x", var_to_checkpoint.x);
    verifyDoubleValue(results[1], "var_to_checkpoint.y", var_to_checkpoint.y);
    verifyIntValue(results[2], "var_to_checkpoint.c1.a", var_to_checkpoint.c1.a);
    verifyDoubleValue(results[3], "var_to_checkpoint.c1.b", var_to_checkpoint.c1.b);
}

TEST_F(FindLeavesTest, composite3) {
    // ARRANGE
    addClassThreeToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassThree");

    ClassThree var_to_checkpoint = {.pos = {5.5, 6.6}, .vel = {7.7, 8.8}};

    // ACT
    FindLeavesVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResult();
    ASSERT_EQ(results.size(), 4);
    verifyDoubleValue(results[0], "var_to_checkpoint.pos[0]", var_to_checkpoint.pos[0]);
    verifyDoubleValue(results[1], "var_to_checkpoint.pos[1]", var_to_checkpoint.pos[1]);
    verifyDoubleValue(results[2], "var_to_checkpoint.vel[0]", var_to_checkpoint.vel[0]);
    verifyDoubleValue(results[3], "var_to_checkpoint.vel[1]", var_to_checkpoint.vel[1]);
}


TEST_F(FindLeavesTest, write_checkpoint_strings) {
    // ARRANGE
    addClassSixToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassSix");

    const char * str_to_test_with =  "A string literal that we probably can't actually checkpoint";


    ClassSix var_to_checkpoint;
    
    var_to_checkpoint.str = "Hello world :)";
    var_to_checkpoint.char_ptr = str_to_test_with;

    // ACT
    FindLeavesVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResult();
    ASSERT_EQ(2, results.size());

    auto leaf = results[0];
    EXPECT_EQ("var_to_checkpoint.char_ptr", leaf.name_stack.toString());

    PointerValue * ptr_val = dynamic_cast <PointerValue *> (leaf.value);
    ASSERT_TRUE(ptr_val != NULL);
    ASSERT_EQ (str_to_test_with, ptr_val->getPointer());    

    leaf = results[1];
    EXPECT_EQ("var_to_checkpoint.str", leaf.name_stack.toString());
    StringValue * str_val = dynamic_cast <StringValue *> (leaf.value);
    ASSERT_TRUE(str_val != NULL);
    ASSERT_EQ ("Hello world :)", str_val->toString());    

}
}