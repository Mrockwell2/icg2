#include <memory>

#include <gtest/gtest.h>
#include <stddef.h>
#include "DataTypeInator.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Type/PointerDataType.hpp"
#include "Type/ArrayDataType.hpp"

// Framework
class DataTypeInatorTest : public ::testing::Test {
    protected:
    DataTypeInator *dataTypeInator;

    DataTypeInatorTest() { dataTypeInator = new DataTypeInator; }
    ~DataTypeInatorTest() { delete dataTypeInator; }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(DataTypeInatorTest, basic) {
    // ARRANGE
    
    // ACT
    std::shared_ptr<const DataType> result = dataTypeInator->resolve("int");

    // ASSERT
    // const SpecifiedPrimitiveDataType<int> * casted_result = dynamic_cast<const SpecifiedPrimitiveDataType<int> *> (result);
    std::shared_ptr<const SpecifiedPrimitiveDataType<int>> casted_result = std::dynamic_pointer_cast<const SpecifiedPrimitiveDataType<int>>(result);
    ASSERT_TRUE(casted_result != NULL);
}


TEST_F(DataTypeInatorTest, Array) {
    // ARRANGE
    
    // ACT
    std::shared_ptr<const DataType> result = dataTypeInator->resolve("int[5]");

    // ASSERT
    ASSERT_TRUE(result != NULL);
    std::shared_ptr<const ArrayDataType> array_result = std::dynamic_pointer_cast<const ArrayDataType> (result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 5);

    std::shared_ptr<const DataType> subtype_result = array_result->getSubType();
    std::shared_ptr<const SpecifiedPrimitiveDataType<int>> subtype_casted_result = std::dynamic_pointer_cast<const SpecifiedPrimitiveDataType<int>> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, MultidimArray) {
    // ARRANGE
    
    // ACT
    std::shared_ptr<const DataType> result = dataTypeInator->resolve("int[5][4]");
    // result->validate();

    // ASSERT

    // Type is Array with element count of 5
    std::shared_ptr<const ArrayDataType> array_result = std::dynamic_pointer_cast<const ArrayDataType> (result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 5);

    // Subtype is array with element count 4
    std::shared_ptr<const DataType> subtype_result = array_result->getSubType();
    array_result = std::dynamic_pointer_cast<const ArrayDataType> (subtype_result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 4);

    // Subtype's subtype is int
    subtype_result = array_result->getSubType();
    std::shared_ptr<const SpecifiedPrimitiveDataType<int>> subtype_casted_result = std::dynamic_pointer_cast<const SpecifiedPrimitiveDataType<int>> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, Pointer) {
    // ARRANGE
    
    // ACT
    std::shared_ptr<const DataType> result = dataTypeInator->resolve("int *");

    // ASSERT

    // Type is a pointer
    std::shared_ptr<const PointerDataType> pointer_result = std::dynamic_pointer_cast<const PointerDataType> (result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype is int
    std::shared_ptr<const DataType> subtype_result = pointer_result->getSubType();
    std::shared_ptr<const SpecifiedPrimitiveDataType<int>> subtype_casted_result = std::dynamic_pointer_cast<const SpecifiedPrimitiveDataType<int>> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, PointerPointer) {
    // ARRANGE
    
    // ACT
    std::shared_ptr<const DataType> result = dataTypeInator->resolve("int **");

    // ASSERT

    // Type is a pointer
    std::shared_ptr<const PointerDataType> pointer_result = std::dynamic_pointer_cast<const PointerDataType> (result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype is pointer
    std::shared_ptr<const DataType> subtype_result = pointer_result->getSubType();
    pointer_result = std::dynamic_pointer_cast<const PointerDataType> (subtype_result);
    ASSERT_TRUE(pointer_result != NULL);;

    // Subtype's subtype is int
    subtype_result = pointer_result->getSubType();
    std::shared_ptr<const SpecifiedPrimitiveDataType<int>> subtype_casted_result =  std::dynamic_pointer_cast<const SpecifiedPrimitiveDataType<int>> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, ArrayPointerPointer) {
    // ARRANGE
    
    // ACT
    std::shared_ptr<const DataType> result = dataTypeInator->resolve("int (**)[5]");

    // ASSERT

    // Type is pointer
    std::shared_ptr<const PointerDataType> pointer_result = std::dynamic_pointer_cast<const PointerDataType> (result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype is pointer
    std::shared_ptr<const DataType> subtype_result = pointer_result->getSubType();
    pointer_result = std::dynamic_pointer_cast<const PointerDataType> (subtype_result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype's subtype is an array[5]
    subtype_result = pointer_result->getSubType();
    std::shared_ptr<const ArrayDataType> array_result = std::dynamic_pointer_cast<const ArrayDataType> (subtype_result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 5);

    // Subtype's subtype's subtype is int
    subtype_result = array_result->getSubType();
    std::shared_ptr<const SpecifiedPrimitiveDataType<int>> subtype_casted_result = std::dynamic_pointer_cast<const SpecifiedPrimitiveDataType<int>> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, PointerPointerArray) {
    // ARRANGE
    
    // ACT
    std::shared_ptr<const DataType> result = dataTypeInator->resolve("int **[5]");

    // ASSERT

    // Type is array[5]
    std::shared_ptr<const ArrayDataType> array_result = std::dynamic_pointer_cast<const ArrayDataType> (result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 5);

    // Subtype is a pointer
    std::shared_ptr<const DataType> subtype_result = array_result->getSubType();
    std::shared_ptr<const PointerDataType> pointer_result = std::dynamic_pointer_cast<const PointerDataType> (subtype_result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype's subtype is pointer
    subtype_result = pointer_result->getSubType();
    pointer_result = std::dynamic_pointer_cast<const PointerDataType> (subtype_result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype's subtype's subtype is int
    subtype_result = pointer_result->getSubType();
    std::shared_ptr<const SpecifiedPrimitiveDataType<int>> subtype_casted_result = std::dynamic_pointer_cast<const SpecifiedPrimitiveDataType<int>> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

