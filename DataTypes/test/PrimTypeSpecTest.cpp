#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>
#include "TypeDictionary.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"

class SpecifiedPrimitiveDataTypeTest : public ::testing::Test {
    protected:
    TypeDictionary *typeDictionary;
    SpecifiedPrimitiveDataTypeTest() { typeDictionary = new TypeDictionary; }
    ~SpecifiedPrimitiveDataTypeTest() { delete typeDictionary; }
    void SetUp() {}
    void TearDown() {}
};

template <typename T>
void validate_getSize(size_t expectedSize) {
    SpecifiedPrimitiveDataType<T> * primTypeSpec = new SpecifiedPrimitiveDataType<T>();
    size_t test_result = primTypeSpec->getSize();
    delete primTypeSpec;
    EXPECT_EQ(expectedSize, test_result);
}

template <typename T>
void validate_printValue(T& testVar, const char* expectedString) {
    std::stringstream ss;
    SpecifiedPrimitiveDataType<T> * primTypeSpec = new SpecifiedPrimitiveDataType<T>();
    primTypeSpec->printValue(ss, &testVar);
    int result = ss.str().compare(expectedString);
    delete primTypeSpec;
    EXPECT_EQ(0, result);
}

template <typename T>
void validate_toString(const char* expectedString) {
    std::string s;
    SpecifiedPrimitiveDataType<T> * primTypeSpec = new SpecifiedPrimitiveDataType<T>();
    s = primTypeSpec->toString();
    int result = s.compare(expectedString);
    delete primTypeSpec;
    EXPECT_EQ(0, result);
}

template <typename T>
void validate_isFloatingPoint(bool expectedResult) {
    SpecifiedPrimitiveDataType<T> * primTypeSpec = new SpecifiedPrimitiveDataType<T>();
    bool result = primTypeSpec->isFloatingPoint();
    delete primTypeSpec;
    EXPECT_EQ(expectedResult, result);
}

template <typename T>
void validate_isSigned(bool expectedResult) {
    SpecifiedPrimitiveDataType<T> * primTypeSpec = new SpecifiedPrimitiveDataType<T>();
    bool result = primTypeSpec->isSigned();
    delete primTypeSpec;
    EXPECT_EQ(expectedResult, result);
}

template <typename T>
void validate_isVoid(bool expectedResult) {
    SpecifiedPrimitiveDataType<T> * primTypeSpec = new SpecifiedPrimitiveDataType<T>();
    bool result = primTypeSpec->isVoid();
    delete primTypeSpec;
    EXPECT_EQ(expectedResult, result);
}

/* ================================================================================
                                         Test Cases
   ================================================================================
*/
TEST_F(SpecifiedPrimitiveDataTypeTest, validate ) {

    /* SpecifiedPrimitiveDataType is always valid, because it is a fundemental, builtin type. */
    SpecifiedPrimitiveDataType<char> * primTypeSpec = new SpecifiedPrimitiveDataType<char>();

    bool test_result;
    test_result = primTypeSpec->validate();

    EXPECT_EQ(true, test_result);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_void) {
    validate_getSize<void>(0);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_char) {
    validate_getSize<char>((int)sizeof(char));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_short) {
    validate_getSize<short>((int)sizeof(short));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_int) {
    validate_getSize<int>((int)sizeof(int));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_long) {
    validate_getSize<long>((int)sizeof(long));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_long_long) {
    validate_getSize<long long>((int)sizeof(long long));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_unsigned_char) {
    validate_getSize<unsigned char>((int)sizeof(unsigned char));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_unsigned_short) {
    validate_getSize<unsigned short>((int)sizeof(unsigned short));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_unsigned_int) {
    validate_getSize<unsigned int>((int)sizeof(unsigned int));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_unsigned_long) {
    validate_getSize<unsigned long>((int)sizeof(unsigned long));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_unsigned_long_long) {
    validate_getSize<unsigned long long>((int)sizeof(unsigned long long));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_wchar_t) {
    validate_getSize<wchar_t>((int)sizeof(wchar_t));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_float) {
    validate_getSize<float>((int)sizeof(float));
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getSize_double) {
    validate_getSize<double>((int)sizeof(double));
}

// -----------------------------------------------------------------------------------------
//                                    assignValue tests
// -----------------------------------------------------------------------------------------

TEST_F(SpecifiedPrimitiveDataTypeTest, assignValue_char) {
    char test_var = '\0';
    SpecifiedPrimitiveDataType<char> * primTypeSpec = new SpecifiedPrimitiveDataType<char>();
    IntegerValue * value = new IntegerValue((int)'A');
    primTypeSpec->assignValue(&test_var, value);
    delete value;
    delete primTypeSpec;
    EXPECT_EQ('A', test_var);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, assignValue_short) {
    short test_var = 0;
    SpecifiedPrimitiveDataType<short> * primTypeSpec = new SpecifiedPrimitiveDataType<short>();
    IntegerValue * value = new IntegerValue(314);
    primTypeSpec->assignValue(&test_var, value);
    delete value;
    delete primTypeSpec;
    EXPECT_EQ(314, test_var);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, assignValue_int) {
    int test_var = 0;
    SpecifiedPrimitiveDataType<int> * primTypeSpec = new SpecifiedPrimitiveDataType<int>();
    IntegerValue * value = new IntegerValue(723);
    primTypeSpec->assignValue(&test_var, value);
    delete value;
    delete primTypeSpec;
    EXPECT_EQ(723, test_var);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, assignValue_long) {
    long test_var = 0;
    SpecifiedPrimitiveDataType<long> * primTypeSpec = new SpecifiedPrimitiveDataType<long>();
    IntegerValue * value = new IntegerValue(723);
    primTypeSpec->assignValue(&test_var, value);
    delete value;
    delete primTypeSpec;
    EXPECT_EQ(723, test_var);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, assignValue_long_long) {
    long long test_var = 0;
    SpecifiedPrimitiveDataType<long long> * primTypeSpec = new SpecifiedPrimitiveDataType<long long>();
    IntegerValue * value = new IntegerValue(723);
    primTypeSpec->assignValue(&test_var, value);
    delete value;
    delete primTypeSpec;
    EXPECT_EQ(723, test_var);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, assignValue_float) {
    float test_var = 0.0;
    SpecifiedPrimitiveDataType<float> * primTypeSpec = new SpecifiedPrimitiveDataType<float>();
    FloatingPointValue * value = new FloatingPointValue(2.7182818);
    primTypeSpec->assignValue(&test_var, value);
    delete value;
    delete primTypeSpec;
    EXPECT_NEAR(2.7182818, test_var, 0.000001);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, assignValue_double) {
    double test_var = 0.0;
    SpecifiedPrimitiveDataType<double> * primTypeSpec = new SpecifiedPrimitiveDataType<double>();
    FloatingPointValue * value = new FloatingPointValue(3.1415927);
    primTypeSpec->assignValue(&test_var, value);
    delete value;
    delete primTypeSpec;
    EXPECT_NEAR(3.1415927, test_var, 0.000001);
}

// -----------------------------------------------------------------------------------------
//                                    printValue tests
// -----------------------------------------------------------------------------------------
TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_char ) {
    char testVar = 'B';
    validate_printValue(testVar, "'B'");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_char_2 ) {
    /* Unprintable chars. */
    char testVar = '\01';
    validate_printValue(testVar, "1");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_short ) {
    short testVar = -1234;
    validate_printValue(testVar, "-1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_int ) {
    int testVar = -1234;
    validate_printValue(testVar, "-1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_long ) {
    long testVar = -1234;
    validate_printValue(testVar, "-1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_long_long ) {
    long long testVar = -1234;
    validate_printValue(testVar, "-1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_unsigned_char ) {
    unsigned char testVar = 'B';
    validate_printValue(testVar, "'B'");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_unsigned_short ) {
    unsigned short testVar = 1234;
    validate_printValue(testVar, "1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_unsigned_int ) {
    unsigned int testVar = 1234;
    validate_printValue(testVar, "1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_unsigned_long ) {
    unsigned long testVar = 1234;
    validate_printValue(testVar, "1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_unsigned_long_long ) {
    unsigned long long testVar = 1234;
    validate_printValue(testVar, "1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_wchar_t ) {
    wchar_t testVar = 1234;
    validate_printValue(testVar, "1234");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_float ) {
    float testVar = 3.14159;
    validate_printValue(testVar, "3.14159");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, printValue_double ) {
    double testVar = 3.14159;
    validate_printValue(testVar, "3.14159");
}

// -----------------------------------------------------------------------------------------
//                                    toString tests
// -----------------------------------------------------------------------------------------
TEST_F(SpecifiedPrimitiveDataTypeTest, toString_void ) {
    validate_toString<void>("void");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_char ) {
    validate_toString<char>("char");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_short ) {
    validate_toString<short>("short");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_int ) {
    validate_toString<int>("int");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_long ) {
    validate_toString<long>("long");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_long_long ) {
    validate_toString<long long>("long long");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_unsigned_char ) {
    validate_toString<unsigned char>("unsigned char");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_unsigned_short ) {
    validate_toString<unsigned short>("unsigned short");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_unsigned_int ) {
    validate_toString<unsigned int>("unsigned int");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_unsigned_long ) {
    validate_toString<unsigned long>("unsigned long");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_unsigned_long_long ) {
    validate_toString<unsigned long long>("unsigned long long");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_wchar_t ) {
    validate_toString<wchar_t>("wchar_t");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_float ) {
    validate_toString<float>("float");
}

TEST_F(SpecifiedPrimitiveDataTypeTest, toString_double ) {
    validate_toString<double>("double");
}

// -----------------------------------------------------------------------------------------
//                                    isFloatingPoint tests
// -----------------------------------------------------------------------------------------
TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_void ) {
    validate_isFloatingPoint<void>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_char ) {
    validate_isFloatingPoint<char>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_short ) {
    validate_isFloatingPoint<short>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_int ) {
    validate_isFloatingPoint<int>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_long ) {
    validate_isFloatingPoint<long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_long_long ) {
    validate_isFloatingPoint<long long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_unsigned_char ) {
    validate_isFloatingPoint<unsigned char>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_unsigned_short ) {
    validate_isFloatingPoint<unsigned short>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_unsigned_int ) {
    validate_isFloatingPoint<unsigned int>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_unsigned_long ) {
    validate_isFloatingPoint<unsigned long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_unsigned_long_long ) {
    validate_isFloatingPoint<unsigned long long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_float ) {
    validate_isFloatingPoint<float>(true);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isFloatingPoint_double ) {
    validate_isFloatingPoint<double>(true);
}

// -----------------------------------------------------------------------------------------
//                                  isSigned tests
// -----------------------------------------------------------------------------------------
TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_void ) {
    validate_isSigned<void>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_char ) {
    validate_isSigned<char>(true);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_short ) {
    validate_isSigned<short>(true);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_int ) {
    validate_isSigned<int>(true);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_long ) {
    validate_isSigned<long>(true);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_long_long ) {
    validate_isSigned<long long>(true);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_unsigned_char ) {
    validate_isSigned<unsigned char>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_unsigned_short ) {
    validate_isSigned<unsigned short>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_unsigned_int ) {
    validate_isSigned<unsigned int>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_unsigned_long ) {
    validate_isSigned<unsigned long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_unsigned_long_long ) {
    validate_isSigned<unsigned long long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_wchar_t ) {
    validate_isSigned<wchar_t>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_float ) {
    validate_isSigned<float>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isSigned_double ) {
    validate_isSigned<double>(false);
}

// -----------------------------------------------------------------------------------------
//                                  isVoid tests
// -----------------------------------------------------------------------------------------
TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_void ) {
    validate_isVoid<void>(true);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_char ) {
    validate_isVoid<char>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_short ) {
    validate_isVoid<short>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_int ) {
    validate_isVoid<int>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_long ) {
    validate_isVoid<long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_long_long ) {
    validate_isVoid<long long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_unsigned_char ) {
    validate_isVoid<unsigned char>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_unsigned_short ) {
    validate_isVoid<unsigned short>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_unsigned_int ) {
    validate_isVoid<unsigned int>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_unsigned_long ) {
    validate_isVoid<unsigned long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_unsigned_long_long ) {
    validate_isVoid<unsigned long long>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_wchar_t ) {
    validate_isVoid<wchar_t>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_float ) {
    validate_isVoid<float>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, isVoid_double ) {
    validate_isVoid<double>(false);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, createInstance_char) {

    SpecifiedPrimitiveDataType<char> * primTypeSpec = new SpecifiedPrimitiveDataType<char>();
    char* test_var = (char*)primTypeSpec->createInstance(1);
    IntegerValue * value = new IntegerValue((int)'A');
    primTypeSpec->assignValue(test_var, value);
    delete value;
    delete primTypeSpec;
    EXPECT_EQ('A', *test_var);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, deleteInstance) {
    // ARRANGE
    SpecifiedPrimitiveDataType<char> * primTypeSpec = new SpecifiedPrimitiveDataType<char>();
    char* test_var = (char*)primTypeSpec->createInstance(1);    

    // ACT
    primTypeSpec->deleteInstance(test_var);

    // ASSERT
    // I guess just assert that it didn't segfault?
    
}

// TEST_F(SpecifiedPrimitiveDataTypeTest, clone) {
//     // ARRANGE
//     SpecifiedPrimitiveDataType<char> * primTypeSpec = new SpecifiedPrimitiveDataType<char>();

//     // ACT
//     DataType * cloned_type = primTypeSpec->clone();

//     // ASSERT
//     ASSERT_TRUE(cloned_type != primTypeSpec);    
//     ASSERT_EQ(cloned_type->toString(), primTypeSpec->toString());    
// }

// -----------------------------------------------------------------------------------------
//                                  GetValue Tests
// -----------------------------------------------------------------------------------------

TEST_F(SpecifiedPrimitiveDataTypeTest, getValue_int) {
    // ARRANGE

    SpecifiedPrimitiveDataType<int> int_type;
    int my_test_int = 8765;

    // ACT
    Value * val = int_type.getValue(&my_test_int);

    // ASSERT
    IntegerValue * int_val = dynamic_cast<IntegerValue *> (val);
    int result = (int) int_val->getIntegerValue();
    ASSERT_EQ(result, my_test_int);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getValue_double) {
    // ARRANGE

    SpecifiedPrimitiveDataType<double> double_type;
    double my_test_double = 5.4321;
    std::stringstream ss;

    // ACT
    Value * val = double_type.getValue(&my_test_double);

    // ASSERT
    FloatingPointValue * float_val = dynamic_cast<FloatingPointValue *> (val);
    double result = (double) float_val->getFloatingPointValue();
    ASSERT_EQ(result, my_test_double);
}

TEST_F(SpecifiedPrimitiveDataTypeTest, getValue_void) {
    // ARRANGE

    SpecifiedPrimitiveDataType<void> void_type;
    double my_test_double = 5.4321;
    std::stringstream ss;

    // ACT
    Value * val = void_type.getValue(&my_test_double);

    // ASSERT
    ASSERT_TRUE(val == NULL);
}