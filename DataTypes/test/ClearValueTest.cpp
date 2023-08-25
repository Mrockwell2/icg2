#include "Type/EnumDictionary.hpp"
#include "Type/NormalStructMember.hpp"
#include "DataTypeTestSupport.hpp"
#include "Type/AllTypes.hpp"
#include "Value/PointerValue.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/StringValue.hpp"

#include "gtest/gtest.h"

#include "Algorithm/DataTypeAlgorithm.hpp"


class ClearValueTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    ClearValueTest() {

    }

    ~ClearValueTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

namespace ClearValue {


TEST_F(ClearValueTest, integer) {
    // ARRANGE
    std::shared_ptr<DataType> type (new IntDataType);
    int var_to_clear = 42;

    // ACT
    DataTypeAlgorithm::clearValue(type,   &var_to_clear);

    // ASSERT
    ASSERT_EQ(0, var_to_clear);
}

TEST_F(ClearValueTest, floating_point) {
    // ARRANGE
    std::shared_ptr<DataType> type (new DoubleDataType);
    double var_to_clear = -42.42;

    // ACT
    DataTypeAlgorithm::clearValue(type,   &var_to_clear);

    // ASSERT
    ASSERT_EQ(0, var_to_clear);
}


TEST_F( ClearValueTest , pointer ) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double*");
    double * var_to_clear = (double *) 0xc0ffee;

    // ACT
    DataTypeAlgorithm::clearValue(type,  &var_to_clear);
    
    // ASSERT
    ASSERT_EQ(NULL, var_to_clear);


}


TEST_F(ClearValueTest, string) {
    // ARRANGE
    std::string expected_value = "I was not able to light on any map or work giving the exact locality of the Castle Dracula, as there are no maps of this country as yet to compare with our own Ordnance Survey maps; but I found that Bistritz, the post town named by Count Dracula, is a fairly well-known place.";
    std::shared_ptr<DataType> type (new StringDataType);

    std::string var_to_clear = expected_value;

    // ACT
    DataTypeAlgorithm::clearValue(type,  &var_to_clear);

    // ASSERT
    ASSERT_EQ(std::string(""), var_to_clear);

}

TEST_F(ClearValueTest, enumerated) {
    // ARRANGE
    addDayOfWeekEnumToTypeDictionary( &dataTypeInator, &enumDictionary);
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("DayOfWeek");
    DayOfWeek var_to_clear = Friday;

    // ACT
    DataTypeAlgorithm::clearValue(type,  &var_to_clear);

    // ASSERT
    ASSERT_EQ(DayOfWeek(0), var_to_clear);


}

TEST_F(ClearValueTest, array) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double[5]");
    double var_to_clear[5] = {1, 2, 3, 4, 5};

    // ACT
    DataTypeAlgorithm::clearValue(type, &var_to_clear);

    // ASSERT
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(0, var_to_clear[i]);
    }



}

class Foo { 
    public:
    int x; 
    double y[5];
};

TEST_F(ClearValueTest, composite) {
    // ARRANGE
    std::shared_ptr<CompositeDataType> type (new CompositeDataType ( "Foo", sizeof(Foo), NULL, NULL));
    type->addRegularMember("x", offsetof(Foo, x), "int");
    type->addRegularMember("y", offsetof(Foo, y), "double[5]");
    type->validate(&dataTypeInator);

    Foo var_to_clear;
    var_to_clear.x = 5;
    for (int i = 0; i < 5; i++) {
        var_to_clear.y[i] = i+5;
    }

    // ACT
    DataTypeAlgorithm::clearValue(type,   &var_to_clear);

    // ASSERT
    ASSERT_EQ(0, var_to_clear.x);
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(0, var_to_clear.y[i]);
    }
}

TEST_F(ClearValueTest, vector) {
    // ARRANGE
    std::shared_ptr<DataType> type (new SpecifiedSequenceDataType<std::vector<int>> ( "std::vector<int>"));
    type->validate(&dataTypeInator);

    std::vector<int> var_to_clear({1, 2, 3});
    

    // ACT
    DataTypeAlgorithm::clearValue(type, &var_to_clear);

    // ASSERT
    ASSERT_EQ(0, var_to_clear.size());
}


}