#include <gtest/gtest.h>
#include <memory>

// Classes under test
#include "Value/StringValue.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/FloatingPointValue.hpp"
#include "Value/PointerValue.hpp"


// Fixture
class ValueTest : public ::testing::Test {
    protected:
    
    ValueTest() {}
    ~ValueTest() {}
    void SetUp() {}
    void TearDown() {}
};

/* ================================================================================
                                         Test Cases
   ================================================================================
*/

TEST_F(ValueTest, integer_type) {
    // ARRANGE
    std::unique_ptr<Value> val(new IntegerValue(42));

    // ACT
    // ASSERT
    ASSERT_EQ(Value::ValueType::INTEGER, val->getValueType());
}

TEST_F(ValueTest, float_type) {
    // ARRANGE
    std::unique_ptr<Value> val(new FloatingPointValue(4.2));

    // ACT
    // ASSERT
    ASSERT_EQ(Value::ValueType::FLOATING_POINT, val->getValueType());
}

TEST_F(ValueTest, ptr_type) {
    // ARRANGE
    std::unique_ptr<Value> val(new PointerValue((void *)0x424242));

    // ACT
    // ASSERT
    ASSERT_EQ(Value::ValueType::POINTER, val->getValueType());
}

TEST_F(ValueTest, string_type) {
    // ARRANGE
    std::unique_ptr<Value> val(new StringValue("Hello world :)"));

    // ACT
    // ASSERT
    ASSERT_EQ(Value::ValueType::STRING, val->getValueType());
}


TEST_F(ValueTest, integer_print) {
    // ARRANGE
    std::unique_ptr<Value> val(new IntegerValue(42));
    std::stringstream ss;

    // ACT
    val->print(ss);

    // ASSERT
    ASSERT_EQ(std::string("42"), ss.str());
}

TEST_F(ValueTest, float_print) {
    // ARRANGE
    std::unique_ptr<Value> val(new FloatingPointValue(4.2));
    std::stringstream ss;

    // ACT
    val->print(ss);

    // ASSERT
    ASSERT_EQ(std::string("4.2"), ss.str());
}

TEST_F(ValueTest, ptr_print) {
    // ARRANGE
    std::unique_ptr<Value> val(new PointerValue((void *)0x424242));
    std::stringstream ss;

    // ACT
    val->print(ss);

    // ASSERT
    ASSERT_EQ(std::string("0x424242"), ss.str());
}

TEST_F(ValueTest, string_print) {
    // ARRANGE
    std::unique_ptr<Value> val(new StringValue("Hello world :)"));
    std::stringstream ss;

    // ACT
    val->print(ss);

    // ASSERT
    ASSERT_EQ(std::string("Hello world :)"), ss.str());
}

TEST_F(ValueTest, integer_tostring) {
    // ARRANGE
    std::unique_ptr<Value> val(new IntegerValue(42));
 
    // ACT
    std::string actual = val->toString();

    // ASSERT
    ASSERT_EQ(std::string("42"), actual);
}

TEST_F(ValueTest, float_tostring) {
    // ARRANGE
    std::unique_ptr<Value> val(new FloatingPointValue(4.2));

    // ACT
    std::string actual = val->toString();

    // ASSERT
    ASSERT_EQ(std::string("4.2"), actual);
}

TEST_F(ValueTest, ptr_tostring) {
    // ARRANGE
    std::unique_ptr<Value> val(new PointerValue((void *)0x424242));
    
    // ACT
    std::string actual = val->toString();

    // ASSERT
    ASSERT_EQ(std::string("0x424242"), actual);
}

TEST_F(ValueTest, string_tostring) {
    // ARRANGE
    std::unique_ptr<Value> val(new StringValue("Hello world :)"));

        
    // ACT
    std::string actual = val->toString();

    // ASSERT
    ASSERT_EQ(std::string("Hello world :)"), actual);
}

TEST_F(ValueTest, string_torawstring) {
    // ARRANGE
    std::unique_ptr<StringValue> val(new StringValue("Hello world :)"));

        
    // ACT
    std::string actual = val->getRawString();

    // ASSERT
    ASSERT_EQ(std::string("Hello world :)"), actual);
}

TEST_F(ValueTest, string_getEscapedString) {
    // ARRANGE
    std::unique_ptr<StringValue> val(new StringValue("Hello world :)"));

    
    // ACT
    std::string actual = val->getEscapedString();

    // ASSERT
    ASSERT_EQ(std::string("\"Hello world :)\""), actual);
}

TEST_F(ValueTest, integer_getinteger) {
    // ARRANGE
    std::unique_ptr<IntegerValue> val(new IntegerValue(42));
 
    // ACT
    int actual = (int) val->getIntegerValue();

    // ASSERT
    ASSERT_EQ(42, actual);
}

TEST_F(ValueTest, integer_getfloat) {
    // ARRANGE
    std::unique_ptr<IntegerValue> val(new IntegerValue(42));
 
    // ACT
    double actual = (double) val->getFloatingPointValue();

    // ASSERT
    ASSERT_EQ(42, actual);
}

TEST_F(ValueTest, floatingpoint_getinteger) {
    // ARRANGE
    std::unique_ptr<FloatingPointValue> val(new FloatingPointValue(4.2));
 
    // ACT
    int actual = (int) val->getIntegerValue();

    // ASSERT
    ASSERT_EQ(4, actual);
}

TEST_F(ValueTest, floatingpoint_getfloat) {
    // ARRANGE
    std::unique_ptr<FloatingPointValue> val(new FloatingPointValue(4.2));
 
    // ACT
    double actual = (double) val->getFloatingPointValue();

    // ASSERT
    ASSERT_EQ(4.2, actual);
}