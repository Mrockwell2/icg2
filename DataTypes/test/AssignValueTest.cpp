#include "Type/EnumDictionary.hpp"

#include "Type/NormalStructMember.hpp"
#include "DataTypeTestSupport.hpp"
#include "Type/AllTypes.hpp"
#include "Value/PointerValue.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/StringValue.hpp"

#include "gtest/gtest.h"

#include "Algorithm/DataTypeAlgorithm.hpp"


class AssignValueTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    AssignValueTest() {

    }

    ~AssignValueTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

namespace AssignValue {


TEST_F(AssignValueTest, integer) {
    // ARRANGE
    std::shared_ptr<DataType> type (new IntDataType);
    int var_to_assign_to = 0;
    IntegerValue val (42);

    // ACT
    DataTypeAlgorithm::assignValue(type, &val,  &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(42, var_to_assign_to);
}

TEST_F(AssignValueTest, integer_type_mismatch) {
    // ARRANGE
    std::shared_ptr<DataType> type (new IntDataType);
    int var_to_assign_to = 10;
    StringValue val ("abc");

    // ACT
    bool result = DataTypeAlgorithm::assignValue(type, &val,  &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(false, result);
    ASSERT_EQ(10, var_to_assign_to);
}

TEST_F(AssignValueTest, floating_point) {
    // ARRANGE
    std::shared_ptr<DataType> type (new DoubleDataType);
    double var_to_assign_to = 0;
    FloatingPointValue val (-42.42);

    // ACT
    DataTypeAlgorithm::assignValue(type, &val,  &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(-42.42, var_to_assign_to);
}

TEST_F(AssignValueTest, floating_point_type_mismatch) {
    // ARRANGE
    std::shared_ptr<DataType> type (new DoubleDataType);
    double var_to_assign_to = 10.0;
    StringValue val ("abc");

    // ACT
    bool result = DataTypeAlgorithm::assignValue(type, &val,  &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(false, result);
    ASSERT_EQ(10.0, var_to_assign_to);
}

TEST_F( AssignValueTest , pointer ) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double*");
    double * var_to_assign_to = (double *) 0xc0ffee;
    PointerValue val ((void *) 0xDEADBEEF);

    // ACT
    DataTypeAlgorithm::assignValue(type, &val, &var_to_assign_to);
    
    // ASSERT
    ASSERT_EQ((double *) 0xdeadbeef, var_to_assign_to);


}

TEST_F(AssignValueTest, pointer_type_mismatch) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double*");
    double * var_to_assign_to = (double *) 0xc0ffee;
    StringValue val ("abc");

    // ACT
    bool result = DataTypeAlgorithm::assignValue(type, &val,  &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(false, result);
    ASSERT_EQ((double *) 0xc0ffee, var_to_assign_to);


}

TEST_F(AssignValueTest, string) {
    // ARRANGE
    std::string expected_value = "I was not able to light on any map or work giving the exact locality of the Castle Dracula, as there are no maps of this country as yet to compare with our own Ordnance Survey maps; but I found that Bistritz, the post town named by Count Dracula, is a fairly well-known place.";
    StringValue val(expected_value);
    std::shared_ptr<DataType> type (new StringDataType);

    std::string var_to_assign_to = expected_value;

    // ACT
    DataTypeAlgorithm::assignValue(type, &val, &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(expected_value, var_to_assign_to);

}

TEST_F(AssignValueTest, string_type_mismatch) {
    // ARRANGE
    std::string expected_value = "I was not able to light on any map or work giving the exact locality of the Castle Dracula, as there are no maps of this country as yet to compare with our own Ordnance Survey maps; but I found that Bistritz, the post town named by Count Dracula, is a fairly well-known place.";
    IntegerValue val(42);
    std::shared_ptr<DataType> type (new StringDataType);

    std::string var_to_assign_to = expected_value;

    // ACT
    bool result = DataTypeAlgorithm::assignValue(type, &val, &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(false, result);
    ASSERT_EQ(expected_value, var_to_assign_to);
}

TEST_F(AssignValueTest, enumerated) {
    // ARRANGE
    addDayOfWeekEnumToTypeDictionary(&dataTypeInator, &enumDictionary);
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("DayOfWeek");
    DayOfWeek var_to_assign_to = Monday;
    IntegerValue val(Friday);

    // ACT
    DataTypeAlgorithm::assignValue(type, &val, &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(Friday, var_to_assign_to);


}

TEST_F(AssignValueTest, enumerated_type_mismatch) {
    // ARRANGE
    addDayOfWeekEnumToTypeDictionary(&dataTypeInator, &enumDictionary);
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("DayOfWeek");
    DayOfWeek var_to_assign_to = Monday;
    StringValue val("Friday");

    // ACT
    bool result = DataTypeAlgorithm::assignValue(type, &val, &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(false, result);
    ASSERT_EQ(Monday, var_to_assign_to);


}

TEST_F(AssignValueTest, array) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double[5]");
    double var_to_assign_to[5] = {1, 2, 3, 4, 5};
    StringValue val ("abc");

    // ACT
    bool result = DataTypeAlgorithm::assignValue(type, &val,  &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(false, result);

}

class Foo {};

TEST_F(AssignValueTest, composite) {
    // ARRANGE
    std::shared_ptr<DataType> type (new CompositeDataType ( "Foo", sizeof(Foo), NULL, NULL));
    type->validate(&dataTypeInator);

    Foo var_to_assign_to;
    StringValue val ("abc");

    // ACT
    bool result = DataTypeAlgorithm::assignValue(type, &val,  &var_to_assign_to);

    // ASSERT
    ASSERT_EQ(false, result);
}


}