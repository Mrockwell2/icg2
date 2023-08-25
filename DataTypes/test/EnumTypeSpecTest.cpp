#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>
#include "TypeDictionary.hpp"
#include "Type/EnumDataType.hpp"
#include "Value/IntegerValue.hpp"

enum DayOfWeek {
    Sunday    = 1,
    Monday    = 2,
    Tuesday   = 3,
    Wednesday = 4,
    Thursday  = 5,
    Friday    = 6,
    Saturday  = 7
};

enum Fruit {
    Apple  = 0,
    Pear   = 1,
    Lemon  = 2,
    Orange = 3
};


// Framework
class EnumDataTypeTest : public ::testing::Test {
    protected:
    TypeDictionary *typeDictionary;
    EnumDictionary *enumDictionary;
    EnumDataTypeTest() {
        typeDictionary = new TypeDictionary;
        enumDictionary = new EnumDictionary;
    }
    ~EnumDataTypeTest() {
        delete typeDictionary;
        delete enumDictionary;
    }
    void SetUp() {}
    void TearDown() {}
};

bool addDayOfWeekEnumToTypeDictionary(TypeDictionary* typeDictionary, EnumDictionary* enumDictionary) {
    bool result = false;
    try {
        EnumDataType * dataType = new EnumDataType( enumDictionary, "DayOfWeek", sizeof(enum DayOfWeek) );
        dataType->addEnumerator( "Sunday",   1);
        dataType->addEnumerator( "Monday",   2);
        dataType->addEnumerator( "Tuesday",  3);
        dataType->addEnumerator( "Wednesday",4);
        dataType->addEnumerator( "Thursday", 5);
        dataType->addEnumerator( "Friday",   6);
        dataType->addEnumerator( "Saturday", 7);

        typeDictionary->addTypeDefinition( "DayOfWeek", dataType );
        result = dataType->validate();

    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}



/* ================================================================================
                                         Test Cases
   ================================================================================
*/
TEST_F(EnumDataTypeTest, addEnumerator_normal ) {

    bool test_result = false;
    test_result = addDayOfWeekEnumToTypeDictionary(typeDictionary, enumDictionary);
    EXPECT_EQ(true, test_result);
}

TEST_F(EnumDataTypeTest, addEnumerator_exception ) {

    std::cout << "===== Expecting an error message about the re-definition of an enumerator type. =====" << std::endl;

    bool test_result = true;

    // Create a EnumDeclaration.
    EnumDataType * dataType = new EnumDataType( enumDictionary, "Fruit", sizeof(enum Fruit) );
    try {
        dataType->addEnumerator( "Apple",  0);
        dataType->addEnumerator( "Pear",   1);
        dataType->addEnumerator( "Lemon",  2);
        dataType->addEnumerator( "Orange", 3);
        dataType->addEnumerator( "Orange", 3);

        typeDictionary->addTypeDefinition( "Fruit", dataType );
        test_result = dataType->validate();

    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        // We are expecting an exception because "Orange" is added twice.
        test_result = true;
    }
    EXPECT_EQ(true, test_result);
}

TEST_F(EnumDataTypeTest, getSize ) {

    EnumDataType * dataType = new EnumDataType( enumDictionary, "DayOfWeek", sizeof(enum DayOfWeek) );
    EXPECT_EQ( (int)sizeof(enum DayOfWeek), dataType->getSize());
}

TEST_F(EnumDataTypeTest, toString ) {

    EXPECT_EQ(true, addDayOfWeekEnumToTypeDictionary(typeDictionary, enumDictionary));

    std::shared_ptr<const DataType> dataType = typeDictionary->lookup("DayOfWeek");
    ASSERT_TRUE(dataType != NULL) ;

    // Print the duplicate CompositeDataType.
    std::string s;
    s = dataType->toString();

    // Check the results.
    EXPECT_EQ("enum {\nSunday = 1,\n"
                      "Monday = 2,\n"
                      "Tuesday = 3,\n"
                      "Wednesday = 4,\n"
                      "Thursday = 5,\n"
                      "Friday = 6,\n"
                      "Saturday = 7}\n",s);
}


TEST_F(EnumDataTypeTest, bad_size) {
    try {
        EnumDataType e1( enumDictionary, "Fruit", 7 );
        FAIL() << "Expected an exception to be thrown." << std::endl;
    } catch (std::exception& ex) {
        // Just want to ensure that there is a message
        EXPECT_TRUE(strlen(ex.what()) > 0);
    }   
}

TEST_F (EnumDataTypeTest, create_instance) {
    // ARRANGE
    EnumDataType e1( enumDictionary, "Fruit", sizeof(int) );
    e1.addEnumerator("Apple", 0);
    e1.addEnumerator("Orange", 1);
    e1.addEnumerator("Pear", 2);

    // ACT
    Fruit * fruit_arr = (Fruit *) e1.createInstance(3);

    // ASSERT
    ASSERT_TRUE(fruit_arr != NULL);

    // I guess just assign to these and make sure they don't segfault?
    fruit_arr[0] = Apple;
    fruit_arr[1] = Orange;
    fruit_arr[2] = Pear;
}

TEST_F (EnumDataTypeTest, delete_instance) {
    // ARRANGE
    EnumDataType e1( enumDictionary, "Fruit", sizeof(int) );
    e1.addEnumerator("Apple", 0);
    e1.addEnumerator("Orange", 1);
    e1.addEnumerator("Pear", 2);
    Fruit * fruit_arr = (Fruit *) e1.createInstance(3);

    // ACT
    e1.deleteInstance(fruit_arr);

    // ASSERT
    // I guess just make sure we don't segfault
}

template <typename T>
void runAssignValueTest(EnumDictionary * enumDictionary) {
    // ARRANGE
    enum Veggie : T {
        Broccoli = 0,
        Asparagus = 1,
        Carrot = 2,
        Eggplant = 3
    };

    EnumDataType e2( enumDictionary, "Veggie", sizeof(Veggie) );
    e2.addEnumerator("Broccoli", 0);
    e2.addEnumerator("Asparagus", 1);
    e2.addEnumerator("Carrot", 2);
    e2.addEnumerator("Eggplant", 3);

    Veggie veg;

    // ACT
    e2.assignValue(&veg, 2);

    // ASSERT
    ASSERT_EQ(Carrot, veg);
}

TEST_F(EnumDataTypeTest, AssignValue_int ) {
    runAssignValueTest<int>(enumDictionary);
}

TEST_F(EnumDataTypeTest, AssignValue_short ) {
    runAssignValueTest<short>(enumDictionary);
}

TEST_F(EnumDataTypeTest, AssignValue_char ) {
    runAssignValueTest<char>(enumDictionary);
}

TEST_F(EnumDataTypeTest, AssignValue_invalid ) {
    // ARRANGE
    enum Veggie  {
        Broccoli = 0,
        Asparagus = 1,
        Carrot = 2,
        Eggplant = 3
    };

    EnumDataType e2( enumDictionary, "Veggie", sizeof(Veggie) );
    e2.addEnumerator("Broccoli", 0);
    e2.addEnumerator("Asparagus", 1);
    e2.addEnumerator("Carrot", 2);
    e2.addEnumerator("Eggplant", 3);

    Veggie veg = Broccoli;

    // ACT
    bool result = e2.assignValue(&veg, 100);

    // ASSERT
    ASSERT_EQ(false, result);
    ASSERT_EQ(Broccoli, veg);
}

template <typename T>
void runGetValueTest(EnumDictionary * enumDictionary) {
    // ARRANGE
    enum Veggie : T {
        Broccoli = 0,
        Asparagus = 1,
        Carrot = 2,
        Eggplant = 3
    };

    EnumDataType e2( enumDictionary, "Veggie", sizeof(Veggie) );
    e2.addEnumerator("Broccoli", 0);
    e2.addEnumerator("Asparagus", 1);
    e2.addEnumerator("Carrot", 2);
    e2.addEnumerator("Eggplant", 3);

    Veggie veg = Eggplant;

    // ACT
    int v = e2.getValue(&veg);

    // ASSERT
    ASSERT_EQ(Eggplant, v);

}

TEST_F(EnumDataTypeTest, GetValue) {
    runGetValueTest<int>(enumDictionary);
}

TEST_F(EnumDataTypeTest, GetValue_short) {
    runGetValueTest<short>(enumDictionary);
}

TEST_F(EnumDataTypeTest, GetValue_char) {
    runGetValueTest<char>(enumDictionary);
}

template <typename T>
void runClearValTest(EnumDictionary * enumDictionary) {
    // ARRANGE
    enum Veggie : T {
        Broccoli = 0,
        Asparagus = 1,
        Carrot = 2,
        Eggplant = 3
    };

    EnumDataType e2( enumDictionary, "Veggie", sizeof(Veggie) );
    e2.addEnumerator("Broccoli", 0);
    e2.addEnumerator("Asparagus", 1);
    e2.addEnumerator("Carrot", 2);
    e2.addEnumerator("Eggplant", 3);

    Veggie veg = Eggplant;

    // ACT
    e2.clearValue(&veg);

    // ASSERT
    ASSERT_EQ(0, veg);
}

TEST_F(EnumDataTypeTest, ClearValue) {
    runClearValTest<int>(enumDictionary);
}

TEST_F(EnumDataTypeTest, ClearValue_short) {
    runClearValTest<short>(enumDictionary);
}

TEST_F(EnumDataTypeTest, ClearValue_char) {
    runClearValTest<char>(enumDictionary);
}

TEST_F(EnumDataTypeTest, LookupEnumName ) {

    // ARRANGE
    EnumDataType * dataType = new EnumDataType( enumDictionary, "DayOfWeek", sizeof(enum DayOfWeek) );
    dataType->addEnumerator( "Sunday",   1);
    dataType->addEnumerator( "Monday",   2);
    dataType->addEnumerator( "Tuesday",  3);
    dataType->addEnumerator( "Wednesday",4);
    dataType->addEnumerator( "Thursday", 5);
    dataType->addEnumerator( "Friday",   6);
    dataType->addEnumerator( "Saturday", 7);

    typeDictionary->addTypeDefinition( "DayOfWeek", dataType );
    dataType->validate();

    // ACT
    std::string name = dataType->lookupEnumeratorName(4);

    // ASSERT
    std::string expected = "Wednesday";
    ASSERT_EQ(expected, name);
}

TEST_F(EnumDataTypeTest, LookupEnumNameNotFound ) {

    // ARRANGE
    EnumDataType * dataType = new EnumDataType( enumDictionary, "DayOfWeek", sizeof(enum DayOfWeek) );
    dataType->addEnumerator( "Sunday",   1);
    dataType->addEnumerator( "Monday",   2);
    dataType->addEnumerator( "Tuesday",  3);
    dataType->addEnumerator( "Wednesday",4);
    dataType->addEnumerator( "Thursday", 5);
    dataType->addEnumerator( "Friday",   6);
    dataType->addEnumerator( "Saturday", 7);

    typeDictionary->addTypeDefinition( "DayOfWeek", dataType );
    dataType->validate();

    // ACT
    std::string name = dataType->lookupEnumeratorName(1000000);

    // ASSERT
    std::string expected = EnumDataType::invalid_str;
    ASSERT_EQ(expected, name);
}

