// Test support
#include "DataTypeTestSupport.hpp"
#include <sstream>
#include <gtest/gtest.h>
#include "Type/SpecifiedSequenceDataType.hpp"

// Class under test
#include "CheckpointAgent/J_CheckpointAgent.hpp"


class J_CheckpointAgentTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    J_CheckpointAgentTest() {

    }

    ~J_CheckpointAgentTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

TEST_F (J_CheckpointAgentTest, whole_format_basic_local) {
    // ARRANGE
    AllocInfo my_int_info ("my_int", dataTypeInator.resolve("int"));
    int * my_int = (int *) my_int_info.getStart();
    *my_int = 42;

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);;
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_int_info);

    // ACT
    checkpoint_agent.dump(ss, alloc_list);
    
    // std::cout << "Result: " << std::endl;
    // std::cout << ss.str() << std::endl;
    
    // ASSERT

    // I don't love this way that I'm testing
    std::string expected(R"(// Variable Declarations.
int my_int ;


// Variable Assignments.
my_int = 42 ;
)");

    EXPECT_EQ(expected, ss.str());

}

TEST_F (J_CheckpointAgentTest, whole_format_basic_extern) {
    // ARRANGE
    int my_int = 42;
    AllocInfo my_int_info ("my_int", dataTypeInator.resolve("int"), &my_int);

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);;
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_int_info);

    // ACT
    checkpoint_agent.dump(ss, alloc_list);
    
    // std::cout << "Result: " << std::endl;
    // std::cout << ss.str() << std::endl;

std::string expected(R"(// Variable Declarations.


// Variable Assignments.
my_int = 42 ;
)");

    EXPECT_EQ(expected, ss.str());

}

TEST_F (J_CheckpointAgentTest, pointer) {
    // ARRANGE
    
    // vars to checkpoint
    int my_int = 42;
    int * my_int_ptr = &my_int;

    AllocInfo my_int_info ("my_int", dataTypeInator.resolve("int"), &my_int);
    AllocInfo my_int_ptr_info ("my_int_ptr", dataTypeInator.resolve("int*"), &my_int_ptr);

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);;
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_int_info);
    alloc_list.push_back(&my_int_ptr_info);

    // ACT
    checkpoint_agent.writeAssignment(ss, &my_int_ptr_info, alloc_list);

    std::string expected("my_int_ptr = &my_int ;\n");

    EXPECT_EQ(expected, ss.str());
}

TEST_F (J_CheckpointAgentTest, classes) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    // vars to checkpoint
    ClassTwo var_to_checkpoint = {.x = 100, .y = 5.5, .c1 = {.a = 5, .b = 1.5}};

    AllocInfo my_alloc_info ("var_to_checkpoint", dataTypeInator.resolve("ClassTwo"), &var_to_checkpoint);

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);;
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_alloc_info);

    // ACT
    checkpoint_agent.writeAssignment(ss, &my_alloc_info, alloc_list);
    
    // std::cout << "Result: " << std::endl;
    // std::cout << ss.str() << std::endl;

    std::string expected(R"(var_to_checkpoint.x = 100 ;
var_to_checkpoint.y = 5.5 ;
var_to_checkpoint.c1.a = 5 ;
var_to_checkpoint.c1.b = 1.5 ;
)");

    EXPECT_EQ(expected, ss.str());
}

TEST_F (J_CheckpointAgentTest, vector_assignment) {
    // ARRANGE

    // var to checkpoint
    std::vector<int> var_to_checkpoint = {10, 20, 30};

    std::shared_ptr<DataType> type (new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));
    type->validate(&dataTypeInator);

    AllocInfo my_alloc_info ("var_to_checkpoint", type, &var_to_checkpoint);

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_alloc_info);

    // ACT
    checkpoint_agent.writeAssignment(ss, &my_alloc_info, alloc_list);

    // ASSERT
    std::string expected =  "RESIZE_STL var_to_checkpoint.size = 3 ;\n"
                            "var_to_checkpoint[0] = 10 ;\n"
                            "var_to_checkpoint[1] = 20 ;\n"
                            "var_to_checkpoint[2] = 30 ;\n";

    EXPECT_EQ(expected, ss.str());
}

TEST_F (J_CheckpointAgentTest, dump_string) {    
    // ARRANGE
    // std::string str_to_checkpoint = "Ra Ra Rasputin";
    AllocInfo my_str_alloc ("str_to_checkpoint", dataTypeInator.resolve("std::string"));
    std::string * str = (std::string *) my_str_alloc.getStart();
    *str = "Ra Ra Rasputin";

    std::vector<AllocInfo *> alloc_list = {&my_str_alloc};
    J_CheckpointAgent checkpoint_agent(&dataTypeInator);
    std::stringstream ss;

    // ACT
    checkpoint_agent.dump(ss, alloc_list);

    // ASSSERT
    std::string expected(R"(// Variable Declarations.
std::string str_to_checkpoint ;


// Variable Assignments.
str_to_checkpoint = "Ra Ra Rasputin" ;
)");

    EXPECT_EQ(expected, ss.str());

}

TEST_F (J_CheckpointAgentTest, whole_format_pointers_galore) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    std::vector<AllocInfo *> alloc_list;
    
    // vars to checkpoint
    ClassTwo my_class = {.x = 100, .y = 5.5, .c1 = {.a = 5, .b = 1.5}};
    AllocInfo my_class_alloc ("var_to_checkpoint", dataTypeInator.resolve("ClassTwo"), &my_class);
    alloc_list.push_back(&my_class_alloc);

    int my_arr[5] = {1, 2, 3, 4, 5};
    AllocInfo my_arr_alloc ("var_to_checkpoint", dataTypeInator.resolve("int[5]"), &my_arr);
    alloc_list.push_back(&my_arr_alloc);

    int * my_arr_3_ptr = &my_arr[3];
    AllocInfo my_arr_3_ptr_alloc ("my_arr_3_ptr", dataTypeInator.resolve("int *"), &my_arr_3_ptr);
    alloc_list.push_back(&my_arr_3_ptr_alloc);

    int (*my_arr_ptr)[5] = &my_arr;
    AllocInfo my_arr_ptr_alloc ("my_arr_ptr", dataTypeInator.resolve("int (*)[5]"), &my_arr_ptr);
    alloc_list.push_back(&my_arr_ptr_alloc);

    int * x_ptr = &my_class.x;
    AllocInfo x_alloc ( "x_ptr", dataTypeInator.resolve("int *"), &x_ptr );
    alloc_list.push_back(&x_alloc);

    double * y_ptr = &my_class.y;
    AllocInfo y_alloc ( "y_ptr", dataTypeInator.resolve("double *"), &y_ptr );
    alloc_list.push_back(&y_alloc);

    ClassOne * c1_ptr = &my_class.c1;
    AllocInfo c1_ptr_alloc ( "c1_ptr", dataTypeInator.resolve("ClassOne *"), &c1_ptr );
    alloc_list.push_back(&c1_ptr_alloc);

    int * a_ptr = &my_class.c1.a;
    AllocInfo a_ptr_alloc ( "a_ptr", dataTypeInator.resolve("int *"), &a_ptr );
    alloc_list.push_back(&a_ptr_alloc);

    double * b_ptr = &my_class.c1.b;
    AllocInfo b_ptr_alloc ( "b_ptr", dataTypeInator.resolve("double *"), &b_ptr );
    alloc_list.push_back(&b_ptr_alloc);

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);;
    std::stringstream ss;

    // ACT
    checkpoint_agent.dump(ss, alloc_list);
    
    // std::cout << "Result: " << std::endl;
    // std::cout << ss.str() << std::endl;

    std::string expected(R"(// Variable Declarations.


// Variable Assignments.
var_to_checkpoint.x = 100 ;
var_to_checkpoint.y = 5.5 ;
var_to_checkpoint.c1.a = 5 ;
var_to_checkpoint.c1.b = 1.5 ;
var_to_checkpoint[0] = 1 ;
var_to_checkpoint[1] = 2 ;
var_to_checkpoint[2] = 3 ;
var_to_checkpoint[3] = 4 ;
var_to_checkpoint[4] = 5 ;
my_arr_3_ptr = &var_to_checkpoint[3] ;
my_arr_ptr = &var_to_checkpoint ;
x_ptr = &var_to_checkpoint.x ;
y_ptr = &var_to_checkpoint.y ;
c1_ptr = &var_to_checkpoint.c1 ;
a_ptr = &var_to_checkpoint.c1.a ;
b_ptr = &var_to_checkpoint.c1.b ;
)");

    EXPECT_EQ(expected, ss.str());
}

TEST_F (J_CheckpointAgentTest, restore_basic_local) {
    // ARRANGE

    // This checkpoint contains 1 local declaration
    // No extern
    std::string checkpoint_str(R"(// Variable Declarations.
int my_int ;



// Variable Assignments.
my_int = 42 ;
)");

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);;
    std::stringstream ss(checkpoint_str);
    std::vector<AllocInfo *> alloc_list;

    // ACT
    alloc_list = checkpoint_agent.restore(ss, alloc_list);
    
    
    // ASSERT
    // AllocList should have 1 member
    ASSERT_EQ(1, alloc_list.size());
    ASSERT_EQ(42, *(int *) alloc_list[0]->getStart() );
    ASSERT_EQ(std::string("my_int"), alloc_list[0]->getName() );
}


TEST_F (J_CheckpointAgentTest, restore_basic_extern) {
    // ARRANGE

    // This checkpoint contains 1 local declaration
    // No extern
    std::string checkpoint_str(R"(// Variable Declarations.


// Clear all allocations to 0.
clear_all_vars();


// Variable Assignments.
my_int = 42 ;
)");

    int my_int = 100;
    AllocInfo my_int_info ("my_int", dataTypeInator.resolve("int"), &my_int);

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);
    std::stringstream ss(checkpoint_str);
    std::vector<AllocInfo *> alloc_list = { &my_int_info };

    // ACT
    alloc_list = checkpoint_agent.restore(ss, alloc_list);    
    
    // ASSERT
    // AllocList should have 1 member
    ASSERT_EQ(1, alloc_list.size());
    ASSERT_EQ(42,  my_int);
}

TEST_F (J_CheckpointAgentTest, restore_pointer) {
    // ARRANGE

    // This checkpoint contains 1 local declaration
    // No extern
    std::string checkpoint_str(R"(// Variable Declarations.


// Clear all allocations to 0.
clear_all_vars();


// Variable Assignments.
my_int = 42 ;
my_int_ptr = &my_int ;
)");

    int my_int = 100;
    int * my_ptr = NULL;
    AllocInfo my_int_info ("my_int", dataTypeInator.resolve("int"), &my_int);
    AllocInfo my_int_ptr_info ("my_int_ptr", dataTypeInator.resolve("int*"), &my_ptr);


    J_CheckpointAgent checkpoint_agent(&dataTypeInator);
    std::stringstream ss(checkpoint_str);
    std::vector<AllocInfo *> alloc_list = { &my_int_info, &my_int_ptr_info};

    // ACT
    alloc_list = checkpoint_agent.restore(ss, alloc_list);    
    
    // ASSERT
    // AllocList should have 2 member
    ASSERT_EQ(2, alloc_list.size());
    ASSERT_EQ(&my_int,  my_ptr);
    ASSERT_EQ(42,  *my_ptr);

}


TEST_F (J_CheckpointAgentTest, restore_class) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    // This checkpoint contains 1 local declaration
    // No extern
    std::string checkpoint_str(R"(var_to_checkpoint.x = 100 ;
var_to_checkpoint.y = 5.5 ;
var_to_checkpoint.c1.a = 5 ;
var_to_checkpoint.c1.b = 1.5 ;
)");

    ClassTwo var_to_checkpoint;
    AllocInfo my_c2_info ("var_to_checkpoint", dataTypeInator.resolve("ClassTwo"), &var_to_checkpoint);


    J_CheckpointAgent checkpoint_agent(&dataTypeInator);
    std::stringstream ss(checkpoint_str);
    std::vector<AllocInfo *> alloc_list = { &my_c2_info };

    // ACT
    alloc_list = checkpoint_agent.restore(ss, alloc_list);    
    
    // ASSERT
    // AllocList should have 1 member
    ASSERT_EQ(1, alloc_list.size());
    ASSERT_EQ(100,  var_to_checkpoint.x);
    ASSERT_EQ(5.5,  var_to_checkpoint.y);
    ASSERT_EQ(5,    var_to_checkpoint.c1.a);
    ASSERT_EQ(1.5,  var_to_checkpoint.c1.b);

}


TEST_F (J_CheckpointAgentTest, whole_restore_pointers_galore) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    std::string checkpoint_str(R"(// Variable Declarations.


// Variable Assignments.
var_to_checkpoint.x = 100 ;
var_to_checkpoint.y = 5.5 ;
var_to_checkpoint.c1.a = 5 ;
var_to_checkpoint.c1.b = 1.5 ;
my_arr[0] = 1 ;
my_arr[1] = 2 ;
my_arr[2] = 3 ;
my_arr[3] = 4 ;
my_arr[4] = 5 ;
my_arr_3_ptr = &my_arr[3] ;
my_arr_ptr = &my_arr ;
x_ptr = &var_to_checkpoint.x ;
y_ptr = &var_to_checkpoint.y ;
c1_ptr = &var_to_checkpoint.c1 ;
a_ptr = &var_to_checkpoint.c1.a ;
b_ptr = &var_to_checkpoint.c1.b ;
)");

    std::vector<AllocInfo *> alloc_list;
    
    // vars to checkpoint
    ClassTwo my_class;
    AllocInfo my_class_alloc ("var_to_checkpoint", dataTypeInator.resolve("ClassTwo"), &my_class);
    alloc_list.push_back(&my_class_alloc);

    int my_arr[5];
    AllocInfo my_arr_alloc ("my_arr", dataTypeInator.resolve("int[5]"), &my_arr);
    alloc_list.push_back(&my_arr_alloc);

    int * my_arr_3_ptr;
    AllocInfo my_arr_3_ptr_alloc ("my_arr_3_ptr", dataTypeInator.resolve("int *"), &my_arr_3_ptr);
    alloc_list.push_back(&my_arr_3_ptr_alloc);

    int (*my_arr_ptr)[5];
    AllocInfo my_arr_ptr_alloc ("my_arr_ptr", dataTypeInator.resolve("int (*)[5]"), &my_arr_ptr);
    alloc_list.push_back(&my_arr_ptr_alloc);

    int * x_ptr;
    AllocInfo x_alloc ( "x_ptr", dataTypeInator.resolve("int *"), &x_ptr );
    alloc_list.push_back(&x_alloc);

    double * y_ptr;
    AllocInfo y_alloc ( "y_ptr", dataTypeInator.resolve("double *"), &y_ptr );
    alloc_list.push_back(&y_alloc);

    ClassOne * c1_ptr;
    AllocInfo c1_ptr_alloc ( "c1_ptr", dataTypeInator.resolve("ClassOne *"), &c1_ptr );
    alloc_list.push_back(&c1_ptr_alloc);

    int * a_ptr;
    AllocInfo a_ptr_alloc ( "a_ptr", dataTypeInator.resolve("int *"), &a_ptr );
    alloc_list.push_back(&a_ptr_alloc);

    double * b_ptr;
    AllocInfo b_ptr_alloc ( "b_ptr", dataTypeInator.resolve("double *"), &b_ptr );
    alloc_list.push_back(&b_ptr_alloc);


    J_CheckpointAgent checkpoint_agent(&dataTypeInator);
    std::stringstream ss(checkpoint_str);

    // ACT
    checkpoint_agent.restore(ss, alloc_list);
    
    ASSERT_EQ(100,  my_class.x);
    ASSERT_EQ(5.5,  my_class.y);
    ASSERT_EQ(5,    my_class.c1.a);
    ASSERT_EQ(1.5,  my_class.c1.b);

    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(i+1, my_arr[i]);
    }

    ASSERT_EQ(&my_arr[3], my_arr_3_ptr);
    ASSERT_EQ(&my_arr, my_arr_ptr);

    ASSERT_EQ(&my_class.x, x_ptr);
    ASSERT_EQ(&my_class.y, y_ptr);
    ASSERT_EQ(&my_class.c1, c1_ptr);
    ASSERT_EQ(&my_class.c1.a, a_ptr);
    ASSERT_EQ(&my_class.c1.b, b_ptr);
}

TEST_F (J_CheckpointAgentTest, restore_string) {    
    // ARRANGE

    std::string checkpoint_str(R"(// Variable Declarations.

// Clear all allocations to 0.
clear_all_vars();


// Variable Assignments.
str_to_checkpoint = "Ra Ra Rasputin" ;
)");

    std::string str_to_checkpoint;

    AllocInfo my_str_alloc ("str_to_checkpoint", dataTypeInator.resolve("std::string"), &str_to_checkpoint);

    std::vector<AllocInfo *> alloc_list = {&my_str_alloc};
    J_CheckpointAgent checkpoint_agent(&dataTypeInator);
    std::stringstream ss(checkpoint_str);

    // ACT
    checkpoint_agent.restore(ss, alloc_list);

    // ASSSERT
    ASSERT_EQ(std::string("Ra Ra Rasputin"), str_to_checkpoint);

}

TEST_F (J_CheckpointAgentTest, vector_resize_cmd) {
    // ARRANGE
    std::string checkpoint_str =  "RESIZE_STL var_to_checkpoint.size = 3 ;\n";
    std::stringstream ss (checkpoint_str);

    // var to checkpoint
    std::vector<int> var_to_checkpoint;

    std::shared_ptr<DataType> type (new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));
    type->validate(&dataTypeInator);

    AllocInfo my_alloc_info ("var_to_checkpoint", type, &var_to_checkpoint);

    J_CheckpointAgent checkpoint_agent(&dataTypeInator);
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_alloc_info);

    // ACT
    checkpoint_agent.restore(ss, alloc_list);

    // ASSERT
    ASSERT_EQ(3, var_to_checkpoint.size());
}