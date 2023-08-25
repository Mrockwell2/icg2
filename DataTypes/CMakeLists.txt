cmake_minimum_required(VERSION 3.1)

set(DATATYPES_SRC
    Value/IntegerValue.cpp
    Value/FloatingPointValue.cpp
    Value/PointerValue.cpp
    Value/StringValue.cpp
    Type/StructMember.cpp
    Type/NormalStructMember.cpp
    Type/StaticStructMember.cpp
    # Type/BitfieldStructMember.cpp
    Type/CompositeDataType.cpp
    Type/EnumDataType.cpp
    Type/ArrayDataType.cpp
    Type/PointerDataType.cpp
    Type/PrimitiveDataType.cpp
    Type/SpecifiedPrimitiveDataType.cpp
    Type/SequenceDataType.cpp
    Type/StringDataType.cpp
    Type/DataType.cpp
    Type/Enumerator.cpp
    Type/EnumDictionary.cpp
    MemoryManagement/AllocInfo.cpp
    MemoryManagement/MemoryManager.cpp
    Utils/LexicalAnalyzer.cpp
    Utils/ParsedDeclaration.cpp
    Utils/MutableDeclaration.cpp
    Utils/ParsedAssignment.cpp
    Algorithm/AssignValue.cpp
    Algorithm/ClearValue.cpp
    Algorithm/DataTypeAlgorithm.cpp
    Algorithm/DataTypeVisitor.cpp
    Algorithm/FindLeaves.cpp
    Algorithm/FindDeps.cpp
    Algorithm/ContainsPointer.cpp
    Algorithm/GetValue.cpp
    Algorithm/LookupAddressAndTypeByName.cpp
    Algorithm/LookupNameByAddressAndType.cpp
    Algorithm/PrintValue.cpp
    Algorithm/ResizeSequence.cpp
    CheckpointAgent/J_CheckpointAgent.cpp
    TypeDictionary.cpp
    DataTypeInator.cpp
)

list(TRANSFORM DATATYPES_SRC PREPEND src/)

add_library(decl STATIC ${DATATYPES_SRC})

target_include_directories(decl PUBLIC include)

if(DECL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()