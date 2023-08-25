#include "io_bar.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, Namespace_checkpoint_restore) {
    // ARRANGE
    std::string checkpoint_str = std::string ("// Variable Declarations.\njackie::j_Bar j_bar ;\n\n\n// Clear all allocations to 0.\nclear_all_vars();\n\n\n// Variable Assignments.\nj_bar.j_foo.my_string = \"Hello world :)\" ;\nj_bar.j_foo.my_ptr = &my_int_to_point_to ;\nj_bar.foo.x = 50 ;\nbar.j_foo.my_string = \"Hello other world!\" ;\nbar.j_foo.my_ptr = &my_int_to_point_to ;\nbar.foo.x = 500 ;\nmy_int_to_point_to = 42 ;\nj_bar_ptr = &j_bar ;\n");

    int my_int_to_point_to = 0;
    memoryManager.declare_var("int my_int_to_point_to", &my_int_to_point_to);
    Bar bar;
    memoryManager.declare_var("Bar bar", &bar);
    jackie::j_Bar * j_bar = (jackie::j_Bar *) memoryManager.declare_var("jackie::j_Bar j_bar");
    memoryManager.declare_var("jackie::j_Bar * j_bar_ptr", &j_bar);

    std::stringstream ss(checkpoint_str);

    // ACT
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    EXPECT_EQ(std::string("Hello other world!"), bar.j_foo.my_string);
    ASSERT_TRUE(bar.j_foo.my_ptr != NULL);
    EXPECT_EQ(42, *bar.j_foo.my_ptr);
    EXPECT_EQ(500, bar.foo.x);

    ASSERT_TRUE(j_bar != NULL);
    EXPECT_EQ(std::string("Hello world :)"), j_bar->j_foo.my_string);
    ASSERT_TRUE(j_bar->j_foo.my_ptr != NULL);
    EXPECT_EQ(42, *j_bar->j_foo.my_ptr);
    EXPECT_EQ(50, j_bar->foo.x);
}

TEST_F(ICGTest, Namespace_checkpoint_write) {
    // ARRANGE
    int my_int_to_point_to = 42;
    memoryManager.declare_var("int my_int_to_point_to", &my_int_to_point_to);

    Bar bar;
    memoryManager.declare_var("Bar bar", &bar);

    bar.j_foo.my_string = "Hello other world!";
    bar.j_foo.my_ptr = &my_int_to_point_to;
    bar.foo.x = 500;

    jackie::j_Bar * j_bar = (jackie::j_Bar *) memoryManager.declare_var("jackie::j_Bar j_bar");

    j_bar->j_foo.my_string = "Hello world :)";
    j_bar->j_foo.my_ptr = &my_int_to_point_to;
    j_bar->foo.x = 50;

    std::stringstream ss;

    // ACT
    memoryManager.write_checkpoint(ss);

    // ASSERT
    std::string expected = std::string ("// Variable Declarations.\njackie::j_Bar j_bar ;\n\n\n// Variable Assignments.\nj_bar.j_foo.my_string = \"Hello world :)\" ;\nj_bar.j_foo.my_ptr = &my_int_to_point_to ;\nj_bar.foo.x = 50 ;\nbar.j_foo.my_string = \"Hello other world!\" ;\nbar.j_foo.my_ptr = &my_int_to_point_to ;\nbar.foo.x = 500 ;\nmy_int_to_point_to = 42 ;\n");

    EXPECT_EQ(expected, ss.str());
}