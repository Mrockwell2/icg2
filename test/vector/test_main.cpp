#include "io_foo.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, vector_restore) {
    // ARRANGE

    std::string checkpoint_str = "RESIZE_STL vec_wrapper.v.size = 5 ; \n"
                                 "vec_wrapper.v[0] = 5 ; \n"
                                 "vec_wrapper.v[1] = 6 ; \n"
                                 "vec_wrapper.v[2] = 7 ; \n"
                                 "vec_wrapper.v[3] = 8 ; \n"
                                 "vec_wrapper.v[4] = 9 ; \n";
    
    // Variable to restore
    VecWrapper vec_wrapper;
    memoryManager.declare_var("VecWrapper vec_wrapper", &vec_wrapper);

    std::stringstream ss(checkpoint_str);

    // ACT
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    ASSERT_EQ(5, vec_wrapper.v.size());

    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(i+5, vec_wrapper.v[i]);
    }
}



