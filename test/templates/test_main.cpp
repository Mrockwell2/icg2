#include "io_foo.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, templates_checkpoint_restore) {
    // ARRANGE
    std::string checkpoint_str = ""
        "// Variable Declarations.\n"
        "// Variable Assignments.\n"
        "RESIZE_STL foo.b1.vec.size = 3 ;\n"
        "foo.b1.vec[0] = 1.1 ;\n"
        "foo.b1.vec[1] = 2.2 ;\n"
        "foo.b1.vec[2] = 3.3 ;\n"
        "foo.b1.a = 100 ;\n"
        "RESIZE_STL foo.b2.vec.size = 5 ;\n"
        "foo.b2.vec[0] = 1 ;\n"
        "foo.b2.vec[1] = 2 ;\n"
        "foo.b2.vec[2] = 3 ;\n"
        "foo.b2.vec[3] = 4 ;\n"
        "foo.b2.vec[4] = 5 ;\n"
        "foo.b2.a = 100.1 ;\n";

    Foo foo;
    memoryManager.declare_var("Foo foo", &foo);

    std::stringstream ss(checkpoint_str);

    // ACT
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    ASSERT_EQ(3, foo.b1.vec.size());
    EXPECT_EQ(1.1, foo.b1.vec[0]);
    EXPECT_EQ(2.2, foo.b1.vec[1]);
    EXPECT_EQ(3.3, foo.b1.vec[2]);
    EXPECT_EQ(100, foo.b1.a);

    ASSERT_EQ(5, foo.b2.vec.size());
    for (int i = 0; i < foo.b2.vec.size(); i++) {
        EXPECT_EQ(i+1, foo.b2.vec[i]);
    }
    EXPECT_EQ(100.1, foo.b2.a);
}

TEST_F(ICGTest, templates_dump_and_restore) {
    // ARRANGE
    Foo * foo_ptr = (Foo *) memoryManager.declare_var("Foo foo");
    memoryManager.declare_var("Foo* foo_ptr", &foo_ptr);

    foo_ptr->b1.a = 100;
    foo_ptr->b1.vec = {1.1, 2.2, 3.3};

    foo_ptr->b2.a = 100.1;
    foo_ptr->b2.vec = {1, 2, 3, 4, 5};

    // dump a checkpoint
    std::stringstream ss;
    memoryManager.write_checkpoint(ss);

    // Change it to something else
    foo_ptr->b1.vec.clear();
    foo_ptr->b2.vec.clear();

    // Restore the first checkpoint
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    ASSERT_EQ(3, foo_ptr->b1.vec.size());
    EXPECT_EQ(1.1, foo_ptr->b1.vec[0]);
    EXPECT_EQ(2.2, foo_ptr->b1.vec[1]);
    EXPECT_EQ(3.3, foo_ptr->b1.vec[2]);

    ASSERT_EQ(5, foo_ptr->b2.vec.size());
    for (int i = 0; i < foo_ptr->b2.vec.size(); i++) {
        EXPECT_EQ(i+1, foo_ptr->b2.vec[i]);
    }
}

TEST_F(ICGTest, declare_templated) {
    Bar<int, double> * b = (Bar<int, double> *) memoryManager.declare_var("Bar<int, double> b");
    memoryManager.declare_var("Bar<int,double> * b_ptr", &b);
    b->vec = {5, 4, 3, 2, 1};

    std::stringstream ss;

    // ACT
    memoryManager.write_checkpoint(ss);
    b->vec.clear();
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    ASSERT_EQ(5, b->vec.size());
    EXPECT_EQ(5, b->vec[0]);
    EXPECT_EQ(4, b->vec[1]);
    EXPECT_EQ(3, b->vec[2]);
    EXPECT_EQ(2, b->vec[3]);
    EXPECT_EQ(1, b->vec[4]);

}   
