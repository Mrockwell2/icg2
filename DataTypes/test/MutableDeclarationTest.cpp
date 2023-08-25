#include <gtest/gtest.h>
#include <stddef.h>
#include "Utils/MutableDeclaration.hpp"
#include <iostream>


TEST(MutableDeclaration, basic_get_declarator) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    // No dimensions
    
    MutableDeclaration builder (type);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, basic_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    // No dimensions
    
    MutableDeclaration builder (type);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, dim1) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a[5]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, dim1_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int[5]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, multidim) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5, 8, 100, 1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a[5][8][100][1]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, multidim_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5, 8, 100, 1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int[5][8][100][1]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pointer) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *a";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pointer_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, multidim_pointer) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1, -1, -1, -1, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *****a";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, multidim_pointer_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1, -1, -1, -1, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*****";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, array_of_pointers) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {3, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "float *baz[3]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, array_of_pointers_abstract) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {3, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "float*[3]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pointer_to_arrays) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {-1, 3};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "float (*baz)[3]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pointer_to_arrays_abstract) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {-1, 3};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "float(*)[3]";
    EXPECT_EQ(result, expected);
}


// int**[3][4]

TEST(MutableDeclaration, complex1) {
    // ARRANGE

    std::string type = "int";
    std::string name = "baz";
    std::vector<int> dims = {3, 4, -1, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int **baz[3][4]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, complex1_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "baz";
    std::vector<int> dims = {3, 4, -1, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int**[3][4]";
    EXPECT_EQ(result, expected);
}


// "int*(*foo)[3][4]"


TEST(MutableDeclaration, complex2) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *(*foo)[3][4]";
    EXPECT_EQ(result, expected);
}


TEST(MutableDeclaration, complex2_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*(*)[3][4]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, invalid_dims) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -100};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, qualified_name) {
    // ARRANGE
    MutableDeclaration builder ("MyNamespace::MyClass a[5]");

    // ACT
    std::string result = builder.getTypeSpecifier();

    // ASSERT
    std::string expected = "MyNamespace::MyClass";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, qualified_name_abstract_declarator) {
    // ARRANGE
    MutableDeclaration builder ("MyNamespace::MyClass a[5]");

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "MyNamespace::MyClass[5]";
    EXPECT_EQ(result, expected);
}


TEST(MutableDeclaration, push_qualifier) {
    // ARRANGE
    MutableDeclaration builder ("MyClass a[5]");

    // ACT
    builder.pushQualifier("MyNamespace");

    // ASSERT
    std::string result = builder.getAbstractDeclarator();
    std::string expected = "MyNamespace::MyClass[5]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pop_qualifier) {
    // ARRANGE
    MutableDeclaration builder ("MyNamespace::MyClass a[5]");

    // ACT
    builder.popQualifier();

    // ASSERT
    std::string result = builder.getAbstractDeclarator();
    std::string expected = "MyClass[5]";
    EXPECT_EQ(result, expected);
}