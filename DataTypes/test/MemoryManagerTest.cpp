#include <gtest/gtest.h>
#include <stddef.h>

#include "Type/CompositeDataType.hpp"

// Class Under Test 
#include "MemoryManagement/MemoryManager.hpp"

// Framework
class MemoryManagerTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    MemoryManager memoryManager;

    MemoryManagerTest() : memoryManager(&dataTypeInator) { }
    ~MemoryManagerTest() {  }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(MemoryManagerTest, declare_var_local) {
    // ARRANGE

    // ACT
    int * my_val = (int *) memoryManager.declare_var("int x");

    // ASSERT
    ASSERT_TRUE(my_val != NULL);


}

TEST_F(MemoryManagerTest, var_exists) {
    // ARRANGE
    int * my_val = (int *) memoryManager.declare_var("int x");

    // ACT
    bool result = memoryManager.var_exists("x");

    // ASSERT
    ASSERT_EQ(true, result);
}

TEST_F(MemoryManagerTest, var_does_not_exist) {
    // ARRANGE
    int * my_val = (int *) memoryManager.declare_var("int x");

    // ACT
    bool result = memoryManager.var_exists("no_such_var");
    
    // ASSERT
    ASSERT_EQ(false, result);
}

TEST_F(MemoryManagerTest, redeclare_existing_var_fails) {
    // ARRANGE
    int * my_val = (int *) memoryManager.declare_var("int x");

    // ACT
    int * same_val = (int *) memoryManager.declare_var("int x");    

    // ASSERT
    ASSERT_EQ(NULL, same_val);
}

TEST_F(MemoryManagerTest, non_existent_type) {
    // ARRANGE

    // ACT
    int * val = (int *) memoryManager.declare_var("no_such_type x");    

    // ASSERT
    ASSERT_EQ(NULL, val);
}

TEST_F(MemoryManagerTest, getDataTypeOf) {
    // ARRANGE
    int * val = (int *) memoryManager.declare_var("int *x[5]");    

    // ACT
    std::shared_ptr<const DataType> type = memoryManager.getDataTypeOf("x");

    // ASSERT
    std::string expected_typename = "int*[5]";
    ASSERT_EQ(expected_typename, type->toString());
}

TEST_F(MemoryManagerTest, getAllocInfoAt) {
    // ARRANGE
    int * val = (int *) memoryManager.declare_var("int *x[5]");    

    // ACT
    AllocInfo * alloc = memoryManager.getAllocInfoAt(val);

    // ASSERT
    ASSERT_TRUE(alloc != NULL);
    ASSERT_EQ(std::string("x"), alloc->getName());
}

TEST_F(MemoryManagerTest, getAllocInfoAt_not_found) {
    // ARRANGE
    int * val = (int *) memoryManager.declare_var("int *x[5]");    

    // ACT
    AllocInfo * alloc = memoryManager.getAllocInfoAt((void *) 0xdeadbeef);

    // ASSERT
    ASSERT_TRUE(alloc == NULL);
}

TEST_F(MemoryManagerTest, getAllocInfoOf) {
    // ARRANGE
    int * val = (int *) memoryManager.declare_var("int x[5]");    

    // ACT
    AllocInfo * alloc = memoryManager.getAllocInfoOf(&val[1]);

    // ASSERT
    ASSERT_TRUE(alloc != NULL);
    ASSERT_EQ(std::string("x"), alloc->getName());
}

TEST_F(MemoryManagerTest, getAllocInfoOf_not_found) {
    // ARRANGE
    int * val = (int *) memoryManager.declare_var("int x[5]");    

    // ACT
    AllocInfo * alloc = memoryManager.getAllocInfoOf((void *) 0xdeadbeef);

    // ASSERT
    ASSERT_TRUE(alloc == NULL);
}

TEST_F(MemoryManagerTest, clear_value) {
    // ARRANGE
    int val[5];
    for (int i = 0; i < 5; i++) {
        val[i] = i+5;
    }
    memoryManager.declare_var("int val[5]", &val);   

    // ACT
    memoryManager.clear_var(&val);

    // ASSERT
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(0, val[i]);
    }  
}

TEST_F(MemoryManagerTest, clear_value_not_found) {
    // ARRANGE
    int val[5];
    for (int i = 0; i < 5; i++) {
        val[i] = i+5;
    }
    memoryManager.declare_var("int val[5]", &val);   

    // ACT
    memoryManager.clear_var((void*) 0xdeadbeef);

    // ASSERT
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(i+5, val[i]);
    }  
}


TEST_F(MemoryManagerTest, clear_value_by_name) {
    // ARRANGE
    int val[5];
    for (int i = 0; i < 5; i++) {
        val[i] = i+5;
    }
    memoryManager.declare_var("int val[5]", &val);   

    // ACT
    memoryManager.clear_var("val");

    // ASSERT
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(0, val[i]);
    }  
}

TEST_F(MemoryManagerTest, clear_value_by_name_not_found) {
    // ARRANGE
    int val[5];
    for (int i = 0; i < 5; i++) {
        val[i] = i+5;
    }
    memoryManager.declare_var("int val[5]", &val);   

    // ACT
    memoryManager.clear_var("no_such_variable");

    // ASSERT
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(i+5, val[i]);
    }  
}

TEST_F(MemoryManagerTest, clear_all_values) {
    // ARRANGE
    int val[5] = {5, 6, 7, 8, 9};
    std::string s = "Hello world";
    double d = 42.42;

    memoryManager.declare_var("int val[5]", &val);   
    memoryManager.declare_var("std::string s", &s);   
    memoryManager.declare_var("double d", &d);   

    // ACT
    memoryManager.clear_all_vars();

    // ASSERT
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(0, val[i]);
    }  
    EXPECT_EQ(std::string(""), s);
    EXPECT_EQ(0.0, d);
}

TEST_F(MemoryManagerTest, delete_var_by_name) {
    // ARRANGE
    double * val = (double *) memoryManager.declare_var("double val");
    ASSERT_EQ(true, memoryManager.var_exists("val"));

    // ACT
    memoryManager.delete_var("val");

    // ASSERT
    ASSERT_EQ(false, memoryManager.var_exists("val")); 
}

TEST_F(MemoryManagerTest, delete_var_by_name_not_found) {
    // ARRANGE
    double * val = (double *) memoryManager.declare_var("double val");
    ASSERT_EQ(true, memoryManager.var_exists("val"));

    // ACT
    memoryManager.delete_var("no_such_var");

    // ASSERT
    ASSERT_EQ(true, memoryManager.var_exists("val")); 
}

TEST_F(MemoryManagerTest, delete_var_by_addr) {
    // ARRANGE
    double * val = (double *) memoryManager.declare_var("double val");
    ASSERT_EQ(true, memoryManager.var_exists("val"));

    // ACT
    memoryManager.delete_var(val);

    // ASSERT
    ASSERT_EQ(false, memoryManager.var_exists("val")); 
}

TEST_F(MemoryManagerTest, delete_var_by_addr_not_found) {
    // ARRANGE
    double * val = (double *) memoryManager.declare_var("double val");
    ASSERT_EQ(true, memoryManager.var_exists("val"));

    // ACT
    memoryManager.delete_var((void *) 0xdeadbeef);

    // ASSERT
    ASSERT_EQ(true, memoryManager.var_exists("val")); 
}

bool constructor_called = false;
bool destructor_called = false;

class MyClass {
    public:
        MyClass () { constructor_called = true; }
        ~MyClass () { destructor_called = true; }
};

void * MyClassAllocator (int n) {
    return new MyClass;
}

void MyClassDeallocator (void * addr) {
    MyClass * temp = (MyClass *) addr;
    delete temp;
}

TEST_F (MemoryManagerTest, declare_calls_constructor_of_local) {
    // ARRANGE
    constructor_called = false;
    destructor_called = false;

    CompositeDataType * type = new CompositeDataType("MyClass", sizeof(MyClass), MyClassAllocator, MyClassDeallocator);
    type->validate(&dataTypeInator);

    dataTypeInator.addToDictionary("MyClass", type);

    // ACT
    MyClass * my_class_instance = (MyClass *) memoryManager.declare_var("MyClass my_class_instance");

    // ASSERT
    ASSERT_EQ(true, constructor_called);
    ASSERT_EQ(true, memoryManager.var_exists("my_class_instance"));
}   

TEST_F (MemoryManagerTest, declare_does_not_call_constructor_of_extern) {
    // ARRANGE
    CompositeDataType * type = new CompositeDataType("MyClass", sizeof(MyClass), MyClassAllocator, MyClassDeallocator);
    dataTypeInator.addToDictionary("MyClass", type);

    MyClass my_class_instance;

    constructor_called = false;
    destructor_called = false;

    // ACT
    memoryManager.declare_var("MyClass my_class_instance", &my_class_instance);

    // ASSERT
    ASSERT_EQ(false, constructor_called);
    ASSERT_EQ(true, memoryManager.var_exists("my_class_instance"));

}   


TEST_F (MemoryManagerTest, delete_calls_destructor_of_local) {
    // ARRANGE
    constructor_called = false;
    destructor_called = false;

    CompositeDataType * type = new CompositeDataType("MyClass", sizeof(MyClass), MyClassAllocator, MyClassDeallocator);
    dataTypeInator.addToDictionary("MyClass", type);

    MyClass * my_class_instance = (MyClass *) memoryManager.declare_var("MyClass my_class_instance");

    // ACT
    memoryManager.delete_var("my_class_instance");

    // ASSERT
    ASSERT_EQ(true, destructor_called);
    ASSERT_EQ(false, memoryManager.var_exists("my_class_instance"));

}   

TEST_F (MemoryManagerTest, delete_does_not_call_destructor_of_extern) {
    // ARRANGE
    constructor_called = false;
    destructor_called = false;

    CompositeDataType * type = new CompositeDataType("MyClass", sizeof(MyClass), MyClassAllocator, MyClassDeallocator);
    dataTypeInator.addToDictionary("MyClass", type);

    MyClass my_class_instance;
    memoryManager.declare_var("MyClass my_class_instance", &my_class_instance);

    // ACT
    memoryManager.delete_var("my_class_instance");

    // ASSERT
    ASSERT_EQ(false, destructor_called);
    ASSERT_EQ(false, memoryManager.var_exists("my_class_instance"));

}   