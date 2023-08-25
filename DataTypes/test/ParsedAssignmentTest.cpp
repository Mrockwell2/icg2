// Class under test
#include "Utils/ParsedAssignment.hpp"
#include "Value/FloatingPointValue.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/StringValue.hpp"
#include <gtest/gtest.h>

TEST (ParsedAssignmentTest, parse_int) {
    // ARRANGE
    // ACT
    ParsedAssignment assignment("my_var = 5;");

    // ASSERT
    std::string expected_name = "my_var";
    EXPECT_EQ(expected_name, assignment.getVariableName());

    Value * value = assignment.getValue();
    ASSERT_TRUE(value != NULL);

    NumericValue * numeric_value = dynamic_cast<NumericValue *> (value);
    ASSERT_TRUE(numeric_value != NULL);
    EXPECT_EQ(5, numeric_value->getIntegerValue());
}

TEST (ParsedAssignmentTest, parse_float) {
    // ARRANGE
    // ACT
    ParsedAssignment assignment("my_var = 5.4;");

    // ASSERT
    std::string expected_name = "my_var";
    EXPECT_EQ(expected_name, assignment.getVariableName());

    Value * value = assignment.getValue();
    ASSERT_TRUE(value != NULL);

    FloatingPointValue * numeric_value = dynamic_cast<FloatingPointValue *> (value);
    ASSERT_TRUE(numeric_value != NULL);
    EXPECT_EQ(5.4, numeric_value->getFloatingPointValue());
}

TEST (ParsedAssignmentTest, parse_string) {
    // ARRANGE
    // ACT
    ParsedAssignment assignment("my_var = \"this is my string literal value\";");

    // ASSERT
    std::string expected_name = "my_var";
    EXPECT_EQ(expected_name, assignment.getVariableName());

    Value * value = assignment.getValue();
    ASSERT_TRUE(value != NULL);

    StringValue * str_value = dynamic_cast<StringValue *> (value);
    ASSERT_TRUE(str_value != NULL);
    EXPECT_EQ("this is my string literal value", str_value->toString());
}

TEST (ParsedAssignmentTest, parse_pointer_val) {
    // ARRANGE
    // ACT
    ParsedAssignment assignment("my_var = &my_other_val;");

    // ASSERT
    std::string expected_name = "my_var";
    EXPECT_EQ(expected_name, assignment.getVariableName());

    Value * value = assignment.getValue();
    ASSERT_TRUE(value != NULL);

    StringValue * str_value = dynamic_cast<StringValue *> (value);
    ASSERT_TRUE(str_value != NULL);
    EXPECT_EQ("&my_other_val", str_value->getRawString());
}

TEST (ParsedAssignmentTest, parse_complex_ptr_val) {
    // ARRANGE
    // ACT
    ParsedAssignment assignment("my_var = &my_other_val.a.b[3].c;");

    // ASSERT
    std::string expected_name = "my_var";
    EXPECT_EQ(expected_name, assignment.getVariableName());

    Value * value = assignment.getValue();
    ASSERT_TRUE(value != NULL);

    StringValue * str_value = dynamic_cast<StringValue *> (value);
    ASSERT_TRUE(str_value != NULL);
    EXPECT_EQ("&my_other_val.a.b[3].c", str_value->getRawString());
}

TEST (ParsedAssignmentTest, parse_error) {
    // ARRANGE
    // ACT
    try {
        ParsedAssignment assignment("my_var = somthing that doesn't make sense;");
        FAIL() << "Expected an exception to be thrown.";
    } catch (std::exception& e) {
        EXPECT_TRUE(strlen(e.what()) > 0);
    }
}