#include "Type/EnumDictionary.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Type/NormalStructMember.hpp"
#include "DataTypeTestSupport.hpp"

#include "gtest/gtest.h"

#include "Algorithm/LookupNameByAddressAndType.hpp"


class LookupNameByAddressVisitorTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    LookupNameByAddressVisitorTest() {

    }

    ~LookupNameByAddressVisitorTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

namespace LookupNameByAddressAndType {

TEST_F(LookupNameByAddressVisitorTest, basic) {
    // ARRANGE
    std::shared_ptr<DataType> int_data_type (new IntDataType);
    int var_to_search = 100;

    int * search_address = &var_to_search;

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, int_data_type);

    // ACT
    bool success = int_data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupNameByAddressVisitorTest, array) {
    // ARRANGE
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("int[5]");
    int var_to_search[5] = {1, 2, 3, 4, 5};

    int * search_address = &var_to_search[3];

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, dataTypeInator.resolve("int"));

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search[3]";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupNameByAddressVisitorTest, composite1) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassOne");
    ClassOne var_to_search = {.a = 5, .b = 1.5};

    void * search_address = &(var_to_search.b);

    std::shared_ptr<const DataType> search_type = dataTypeInator.resolve("double");

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search.b";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupNameByAddressVisitorTest, composite_array) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassOne[4][3]");
    ClassOne var_to_search[4][3];

    void * search_address = &(var_to_search[2][2].b);

    std::shared_ptr<const DataType> search_type = dataTypeInator.resolve("double");

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search[2][2].b";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupNameByAddressVisitorTest, composite_array2) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search[2][2].c1.b);

    std::shared_ptr<const DataType> search_type = dataTypeInator.resolve("double");

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search[2][2].c1.b";
    ASSERT_EQ(expected, actual);
}

// Explanation for the next two tests
// 

TEST_F(LookupNameByAddressVisitorTest, search_type_ambiguous) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search[2][2].c1);

    std::shared_ptr<const DataType> search_type = dataTypeInator.resolve("ClassOne");

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search[2][2].c1";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupNameByAddressVisitorTest, search_type_ambiguous2) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search[2][2].c1);

    std::shared_ptr<const DataType> search_type = dataTypeInator.resolve("int");

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search[2][2].c1.a";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupNameByAddressVisitorTest, search_type_ambiguous3) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search);

    std::shared_ptr<const DataType> search_type = dataTypeInator.resolve("int");

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search[0][0].x";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupNameByAddressVisitorTest, search_type_ambiguous4) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search);

    std::shared_ptr<const DataType> search_type = dataTypeInator.resolve("ClassTwo");

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search[0][0]";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupNameByAddressVisitorTest, search_type_ambiguous5) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    std::shared_ptr<const DataType> data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search);

    std::shared_ptr<const DataType> search_type = dataTypeInator.resolve("ClassTwo[3]");

    LookupNameByAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult();
    std::string expected = "var_to_search[0]";
    ASSERT_EQ(expected, actual);
}
}