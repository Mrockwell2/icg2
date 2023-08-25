cmake_minimum_required(VERSION 3.1)

set(ICG_SRC
    icg_main.cpp
    utils.cpp
    ICGTemplateEngine/ICGTemplateEngine.cpp
    IntermediateRepresentation/ClassInfo.cpp
    IntermediateRepresentation/FieldInfo.cpp
    IntermediateRepresentation/STLDeclInfo.cpp
    IntermediateRepresentation/ASTInfo.cpp
    JClang/JClang.cpp
    JClang/Traversal.cpp
    ASTFilter/ASTFilter.cpp
)

list(TRANSFORM ICG_SRC PREPEND src/)

add_executable (icg ${ICG_SRC})

target_include_directories(icg PUBLIC include)
target_include_directories(icg PUBLIC ${CMAKE_SOURCE_DIR}/extern/json)
