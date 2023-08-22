#include "io_header.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"

#include <fstream>


TEST_F(ICGTest, PartialCheckpointDump) {
    // ARRANGE
	std::ifstream cp_file;
	int index = 0;
	std::string refStr[8] = { "// Variable Declarations.", 
							  "double *dub_ptr ;", 
							  "double dub ;", 
							  "// Clear all allocations to 0.", 
							  "clear_all_vars();", 
							  "// Variable Assignments.", 
							  "dub_ptr = &dub ;", 
							  "dub = 5 ;" };
	
    MyClass * my_class_instance;
	Foo * foo_instance;
	double **d_ptr, *d;
    my_class_instance = (MyClass *) memoryManager.declare_var("MyClass a");
    foo_instance = (Foo *) memoryManager.declare_var("Foo ptr");
	d_ptr = (double **) memoryManager.declare_var("double * dub_ptr");
	d = (double *) memoryManager.declare_var("double dub");

	foo_instance->foo_int = 5;
	foo_instance->my_arr[0] = 1.2;
	foo_instance->my_arr[1] = 3.4;
	foo_instance->my_arr[2] = 5.6;
	foo_instance->my_arr[3] = 7.8;
	foo_instance->my_arr[4] = 9.0;

	my_class_instance->my_int = 10;
	my_class_instance->my_nested_class = *foo_instance;

	*d = 5.0;
	*d_ptr = d;
    
    // ACT
    memoryManager.write_partial_checkpoint("checkpoint.txt", "dub_ptr");
	cp_file.open("checkpoint.txt");

    // ASSERT
	ASSERT_TRUE(cp_file.is_open());

	for( std::string line; std::getline(cp_file, line); ) {
		if(!line.empty()) {
			ASSERT_EQ(0,strcmp_IgnoringWhiteSpace(line.c_str(), refStr[index].c_str()));
			index++;
		}
	}
	
    // for (int i = 0; i < 5; i++) {
    //     ASSERT_EQ(my_class_instance->my_nested_class.my_arr[i], i+5);
    // }
}