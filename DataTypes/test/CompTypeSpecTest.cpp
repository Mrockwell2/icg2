
#include <gtest/gtest.h>
#include <stddef.h>

#include "DataTypeInator.hpp"

#include "Type/CompositeDataType.hpp"
#include "Type/PrimitiveDataType.hpp"

#include "DataTypeTestSupport.hpp"

// Framework
class CompositeDataTypeTest : public ::testing::Test {
    protected:
    DataTypeInator *dataTypeInator;
    CompositeDataTypeTest() { dataTypeInator = new DataTypeInator; }
    ~CompositeDataTypeTest() { delete dataTypeInator; }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(CompositeDataTypeTest, validate ) {

    /* Requirement: CompositeDataType::validate() method shall attempt to resolve
       all of it's prerequisite type names to actual DataTypes. If it succeeds,
       it shall return true, otherwise it shall return false. */

    bool test_result;

    // Create a CompositeDataType.
    /* Note that this DataType is dependent on the types "int" and "double". Both
       both of these named type dependencies must be resolvable, via the DataTypeInator,
       to their respective DataTypes. */

    test_result = addClassOneToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

    test_result = addClassTwoToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

    test_result = addClassThreeToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

    test_result = addClassFourToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

    test_result = addClassFiveToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

}

TEST_F(CompositeDataTypeTest, toString_1 ) {

    EXPECT_EQ(true, addClassOneToTypeDictionary( dataTypeInator ));

    std::shared_ptr<const DataType> dataType = dataTypeInator->resolve("ClassOne");

    ASSERT_TRUE(dataType != NULL) ;

    // Print the duplicate CompositeDataType.
    std::string s;
    s = dataType->toString();

    // Check the results.
    EXPECT_EQ("composite {\nint a;\ndouble b;\n}\n", s);
}

TEST_F(CompositeDataTypeTest, toString_2 ) {

    EXPECT_EQ(true, addClassOneToTypeDictionary( dataTypeInator ));
    EXPECT_EQ(true, addClassTwoToTypeDictionary( dataTypeInator ));

    std::shared_ptr<const DataType> dataType = dataTypeInator->resolve("ClassTwo");
    ASSERT_TRUE(dataType != NULL) ;

    // Print the duplicate CompositeDataType.
    std::stringstream ss;
    ss << dataType->toString();

    // Check the results.
    EXPECT_EQ("composite {\nint x;\ndouble y;\nClassOne c1;\n}\n", ss.str());
}

TEST_F(CompositeDataTypeTest, toString_3 ) {

    EXPECT_EQ(true, addClassThreeToTypeDictionary( dataTypeInator ));

    std::shared_ptr<const DataType> dataType = dataTypeInator->resolve("ClassThree");
    ASSERT_TRUE(dataType != NULL) ;

    // Print the duplicate CompositeDataType.
    std::stringstream ss;
    ss << dataType->toString();

    // Check the results.
    EXPECT_EQ("composite {\ndouble pos[2];\ndouble vel[2];\n}\n", ss.str());
}


TEST_F(CompositeDataTypeTest, getSize ) {

    /* Requirement: CompositeDataType::getSize() shall return the size passed in
       via the constructor. */

    CompositeDataType * compTypeSpec = new CompositeDataType( "ClassOne",
                                                              sizeof(ClassOne),
                                                              &construct<ClassOne>,
                                                              &destruct<ClassOne>);

    compTypeSpec->addRegularMember( "a", offsetof(ClassOne, a), "int");
    compTypeSpec->addRegularMember( "b", offsetof(ClassOne, b), "double");

    EXPECT_EQ( sizeof(ClassOne), compTypeSpec->getSize());

}