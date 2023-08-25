#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>

#include "TypeDictionary.hpp"
#include "Type/PointerDataType.hpp"
#include "Value/PointerValue.hpp"

// Framework
class PointerDataTypeTest : public ::testing::Test {
    protected:
    DataTypeInator *dataTypeInator;
    PointerDataTypeTest() { dataTypeInator = new DataTypeInator; }
    ~PointerDataTypeTest() { delete dataTypeInator; }
    void SetUp() {}
    void TearDown() {}
};

/* ================================================================================
                                         Test Cases
   ================================================================================
*/




TEST_F( PointerDataTypeTest , getSize ) {

    // Create a PointerDataType.
    PointerDataType* ptrTypeSpec = new PointerDataType( "long**");

    // Verify that getSize returns the size of a pointer.
    EXPECT_EQ( sizeof(void*), ptrTypeSpec->getSize());
}


TEST_F( PointerDataTypeTest , validate_1 ) {

    // Create a PointerDataType.
    PointerDataType* ptrTypeSpec = new PointerDataType( "double");

    // Validate the Type.
    bool validation_result = ptrTypeSpec->validate(dataTypeInator);

    // Verify that validation succeeded.
    ASSERT_EQ(true, validation_result);

}

TEST_F( PointerDataTypeTest , validate_2 ) {

    std::cout << "===== Expecting an error message about an undefined type. =====" << std::endl;

    // Create a PointerDataType.
    PointerDataType* ptrTypeSpec = new PointerDataType( "Undefined_Type");

    // Validate the Type.
    bool validation_result = ptrTypeSpec->validate(dataTypeInator);

    // Verify that validation failed.
    ASSERT_EQ(false, validation_result);

}

TEST_F( PointerDataTypeTest , getDereferencedType_1 ) {

    // Create type: double**
    PointerDataType * ptrTypeSpec;
    bool constructor_result = true;
    try {
        ptrTypeSpec = new PointerDataType( "double*");
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }
    ASSERT_EQ(true, constructor_result);

    // Validate the Type.
    bool validation_result = ptrTypeSpec->validate(dataTypeInator);
    ASSERT_EQ(true, validation_result);

    // Dereference the type.
    std::shared_ptr<const DataType> dereferencedPtrTypeSpec;
    bool  dereference_result = true;
    try {
        dereferencedPtrTypeSpec = ptrTypeSpec->getSubType();
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        dereference_result = false;
    }
    ASSERT_EQ(true, dereference_result);

    // Result of the dereference should be: double*
    std::stringstream ss;
    ss << dereferencedPtrTypeSpec->toString();
    int comparision_result = ss.str().compare("double*");

    EXPECT_EQ(0, comparision_result);

}

TEST_F( PointerDataTypeTest , getDereferencedType_2 ) {

    // Create type: double*
    PointerDataType * ptrTypeSpec;
    bool constructor_result = true;
    try {
        ptrTypeSpec = new PointerDataType( "double");
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }
    ASSERT_EQ(true, constructor_result);

    // Validate the Type.
    bool validation_result = ptrTypeSpec->validate(dataTypeInator);
    ASSERT_EQ(true, validation_result);

    // Dereference the type.
    std::shared_ptr<const DataType> dereferencedPtrTypeSpec;
    bool  dereference_result = true;
    try {
        dereferencedPtrTypeSpec = ptrTypeSpec->getSubType();
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        dereference_result = false;
    }
    ASSERT_EQ(true, dereference_result);

    // Result of the dereference should be: double
    std::stringstream ss;
    ss << dereferencedPtrTypeSpec->toString();
    int comparision_result = ss.str().compare("double");

    EXPECT_EQ(0, comparision_result);
}
