#include "Algorithm/DataTypeAlgorithm.hpp"
#include "Type/AllTypes.hpp"

#include "DataTypeInator.hpp"
#include "DataTypeTestSupport.hpp"

#include <gtest/gtest.h>

class ResizeSequenceTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;

    ResizeSequenceTest() {}
    ~ResizeSequenceTest() {}
};

TEST_F( ResizeSequenceTest , bare_sequence ) {

    // ARRANGE    
    std::shared_ptr<SequenceDataType> type = std::make_shared<SpecifiedSequenceDataType<std::vector<int>>>("std::vector<int>");
    type->validate(&dataTypeInator);

    std::vector<int> vec({1, 2, 3, 4, 5});

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &vec, "size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(10, vec.size());
}

class VecClass {
    public:
    std::vector<int> v;
};

void addVecClassToDataTypeInator(DataTypeInator& dataTypeInator) {

    SequenceDataType * vecType = new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>");

    vecType->validate(&dataTypeInator);
    dataTypeInator.addToDictionary("std::vector<int>", vecType);

    CompositeDataType * vecClassSpec =
        new CompositeDataType( "VecClass", sizeof(VecClass), &construct<VecClass>, &destruct<VecClass>);
        vecClassSpec->addRegularMember( "v", offsetof(VecClass, v), "std::vector<int>");

    dataTypeInator.addToDictionary("VecClass", vecClassSpec);

    vecClassSpec->validate(&dataTypeInator);
}

TEST_F( ResizeSequenceTest , sequence_nested_in_class ) {

    // ARRANGE    
    addVecClassToDataTypeInator(dataTypeInator);

    VecClass var[5];
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("VecClass[4]");
    ASSERT_TRUE(type != NULL);

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[3].v.size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(10, var[3].v.size());


}

TEST_F( ResizeSequenceTest , nested_sequence ) {

    // ARRANGE    
    dataTypeInator.addToDictionary("std::vector<int>", new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));
    dataTypeInator.addToDictionary("std::vector<std::vector<int>>", new SpecifiedSequenceDataType<std::vector<std::vector<int>>>("std::vector<std::vector<int>>"));
    dataTypeInator.validateDictionary();

    std::vector<std::vector<int>> var (5);
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("std::vector<std::vector<int>>");
    ASSERT_TRUE(type != NULL);

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[3].size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(5, var.size());
    EXPECT_EQ(0, var[0].size());
    EXPECT_EQ(10, var[3].size());
}

class VecClass_static {
    public:
    static std::vector<int> v;
};

std::vector<int> VecClass_static::v;

void addVecClass_staticToDataTypeInator(DataTypeInator& dataTypeInator) {

    dataTypeInator.addToDictionary("std::vector<int>", new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));

    CompositeDataType * vecClassSpec =
        new CompositeDataType( "VecClass_static", sizeof(VecClass_static), &construct<VecClass_static>, &destruct<VecClass_static>);
        vecClassSpec->addStaticMember( "v", &VecClass_static::v, "std::vector<int>");

    dataTypeInator.addToDictionary("VecClass_static", vecClassSpec);

    vecClassSpec->validate(&dataTypeInator);
}

TEST_F( ResizeSequenceTest , static_sequence_class ) {
    // ARRANGE    
    addVecClass_staticToDataTypeInator(dataTypeInator);

    VecClass_static var;
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("VecClass_static");
    ASSERT_TRUE(type != NULL);

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "v.size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(10, var.v.size());
}

TEST_F(ResizeSequenceTest, primitive) {
    // ARRANGE
    std::shared_ptr<DataType> type = std::make_shared<DoubleDataType>();
    int var;

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, string) {
    // ARRANGE
    std::shared_ptr<DataType> type = std::make_shared<StringDataType>();
    std::string var;

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "size", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, array) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double[5]");
    double var[5];


    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[2].size", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, array_bad_index) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double[5]");
    double var[5];


    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "not_an_index", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, array_out_of_bounds) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double[5]");
    double var[5];


    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[100]", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}


TEST_F(ResizeSequenceTest, bare_pointer) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("int *");
    int * var;

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, enum) {
    // ARRANGE
    EnumDictionary enumDictionary;
    addDayOfWeekEnumToTypeDictionary(&dataTypeInator, &enumDictionary);
    DayOfWeek var = Monday;
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("DayOfWeek");

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}