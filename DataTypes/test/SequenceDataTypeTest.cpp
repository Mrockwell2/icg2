#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>
#include "DataTypeInator.hpp"
#include "Type/SpecifiedSequenceDataType.hpp"

#include <vector>
#include <stack>

// Framework
class SequenceDataTypeTest : public ::testing::Test {
    protected:
    DataTypeInator *dataTypeInator;
    SequenceDataTypeTest() { dataTypeInator = new DataTypeInator; }
    ~SequenceDataTypeTest() { delete dataTypeInator; }
    void SetUp() {}
    void TearDown() {}
};

/* ================================================================================
                                         Test Cases
   ================================================================================
*/

TEST_F( SequenceDataTypeTest, validate ) {

    // ARRANGE
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>" );

    // ACT
    bool validation_result = type->validate(dataTypeInator);

    // ASSERT
    ASSERT_EQ(true, validation_result);
}

TEST_F( SequenceDataTypeTest, validate_fails ) {

    // ARRANGE
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "not_an_stl" );

    // ACT
    bool validation_result = type->validate(dataTypeInator);

    // ASSERT
    ASSERT_EQ(false, validation_result);
}

TEST_F( SequenceDataTypeTest, validate_fails2 ) {

    // ARRANGE
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<no_such_type>" );

    // ACT
    bool validation_result = type->validate(dataTypeInator);

    // ASSERT
    ASSERT_EQ(false, validation_result);
}

TEST_F( SequenceDataTypeTest, allocate ) {

    // ARRANGE
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");

    bool validation_result = type->validate(dataTypeInator);
    ASSERT_EQ(true, validation_result);

    // ACT
    std::vector<int> * v = (std::vector<int> *) type->createInstance(1);

    // ASSERT
    // just make sure it doesn't segfault? not really sure
    for (int i = 0; i < 10; i++) {
        v->push_back(i);
    }

    ASSERT_EQ(10, v->size());

    type->deleteInstance(v);

}

TEST_F( SequenceDataTypeTest, getSubType ) {

    // Create a SequenceDataType.
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");

    bool validation_result = type->validate(dataTypeInator);
    ASSERT_EQ(true, validation_result);

    std::shared_ptr<const DataType> subType = type->getSubType();
    ASSERT_EQ("int", subType->toString());
}

TEST_F( SequenceDataTypeTest , getSize ) {

    // Create a type for vector.
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(dataTypeInator);

    // Verify that getSize returns the size of the STL container
    EXPECT_EQ( sizeof(std::vector<int>), type->getSize());
}


TEST_F( SequenceDataTypeTest , nested ) {
    std::vector<std::vector<int>> a;

    // Create a type for vector.
    SequenceDataType * type1 = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    SequenceDataType * type2 = new SpecifiedSequenceDataType<std::vector<std::vector<int>>>(  "std::vector<std::vector<int>>" );

    dataTypeInator->addToDictionary("std::vector<int>", type1);
    dataTypeInator->addToDictionary("std::vector<std::vector<int>>", type2);

    ASSERT_EQ(true, dataTypeInator->validateDictionary());
    ASSERT_EQ(true, type1->validate(dataTypeInator));
    ASSERT_EQ(true, type2->validate(dataTypeInator));

    std::shared_ptr<const DataType> subType = type2->getSubType();
    ASSERT_EQ("std::vector<int>", subType->toString());

    std::shared_ptr<const SequenceDataType>  subTypeCasted = std::dynamic_pointer_cast<const SequenceDataType > (subType);
    std::shared_ptr<const DataType> nestedSubType = subTypeCasted->getSubType();
    ASSERT_TRUE(nestedSubType != NULL);
    ASSERT_EQ("int", nestedSubType->toString());
}

TEST_F( SequenceDataTypeTest , getAddresses ) {

    // ARRANGE    
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(dataTypeInator);

    std::vector<int> vec({1, 2, 3, 4, 5});

    // Verify that getSize returns the size of the STL container
    auto elems = type->getElementAddresses(&vec);
    EXPECT_EQ(vec.size(), elems.size());
    for (int i = 0; i < elems.size(); i++) {
        EXPECT_EQ(vec[i], *(int*)elems[i]);
    }
}

TEST_F( SequenceDataTypeTest , getAddressesChanges ) {

    // ARRANGE    
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(dataTypeInator);

    std::vector<int> vec({1, 2, 3});

    // Verify that getSize returns the size of the STL container
    auto elems = type->getElementAddresses(&vec);
    EXPECT_EQ(vec.size(), elems.size());
    for (int i = 0; i < elems.size(); i++) {
        EXPECT_EQ(vec[i], *(int*)elems[i]);
    }

    // ACT
    // Do it again with different elems
    // Push size past a power of 2 so that the underlying container will reallocate
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);
    vec.push_back(7);

    elems = type->getElementAddresses(&vec);
    EXPECT_EQ(vec.size(), elems.size());
    for (int i = 0; i < elems.size(); i++) {
        EXPECT_EQ(vec[i], *(int*)elems[i]);
    }
}

TEST_F( SequenceDataTypeTest , getAddressesDeque ) {

    // ARRANGE    
    SequenceDataType * type = new SpecifiedSequenceDataType<std::deque<int>>(  "std::deque<int>");
    type->validate(dataTypeInator);

    std::deque<int> deque({1, 2, 3, 4, 5});

    // Verify that getSize returns the size of the STL container
    auto elems = type->getElementAddresses(&deque);
    EXPECT_EQ(deque.size(), elems.size());

    int i = 0;
    for (auto deque_elem : deque) {
        EXPECT_EQ(deque_elem, *(int*)elems[i++]);
    }
}

TEST_F( SequenceDataTypeTest , resize ) {

    // ARRANGE    
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(dataTypeInator);

    std::vector<int> vec;

    // ACT
    type->resize(&vec, 10);

    // ASSERT
    EXPECT_EQ(vec.size(), 10);
}

TEST_F( SequenceDataTypeTest , resize_smaller ) {

    // ARRANGE    
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(dataTypeInator);

    std::vector<int> vec({1, 2, 3, 4, 5});

    // ACT
    type->resize(&vec, 3);

    // ASSERT
    ASSERT_EQ(vec.size(), 3);

    auto elems = type->getElementAddresses(&vec);
    EXPECT_EQ(3, elems.size());

    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(i+1, *(int*)elems[i]);
    }
}

TEST_F( SequenceDataTypeTest , clear ) {

    // ARRANGE    
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(dataTypeInator);

    std::vector<int> vec ({1, 2, 3, 4, 5});

    // ACT
    type->clear(&vec);

    // ASSERT
    EXPECT_EQ(vec.size(), 0);
}
