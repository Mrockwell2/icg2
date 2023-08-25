#pragma once

#include <map>
#include <string>

std::string io_src = std::string(R"(
#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include "DataTypeInator.hpp"
#include "Type/EnumDictionary.hpp"
#include "Type/AllTypes.hpp"

#include "{{filename}}"

// An Allocator
template <typename T>
void* construct (int num) {
    T* temp = (T*)calloc( num, sizeof(T));
    for (int ii=0 ; ii<num ; ii++) { new( &temp[ii]) T(); }
    return ((void *)temp);
}

// A deAllocator
// Object constructed with placement new must call the destructor explicitly (https://isocpp.org/wiki/faq/dtors#placement-new)
template <typename T>
void destruct (void* address) {
    T* temp = static_cast<T*> (address);
    temp->~T();
}

{{list_classes_class_decl}}

void populate_type_dictionary(DataTypeInator * dataTypeInator) {
    {{list_classes_call_class_decl}}

    {{list_stls_stl_decl}}

    dataTypeInator->validateDictionary();
}
)");

std::string call_class_decl = std::string(R"( 
    add{{ClassName_mangled}}toDictionary(dataTypeInator);
)");

std::string class_decl = std::string(R"(
void add{{ClassName_mangled}}toDictionary(DataTypeInator* dataTypeInator) {

    CompositeDataType* {{ClassName_mangled}}TypeSpec = new CompositeDataType("{{ClassName}}", sizeof({{ClassName}}), &construct<{{ClassName}}>, &destruct<{{ClassName}}> );
    using type_to_add = {{ClassName}};

        {{list_fields_field_decl}}

    dataTypeInator->addToDictionary("{{ClassName}}", {{ClassName_mangled}}TypeSpec);
}
)");

std::string field_decl = std::string(R"(
    {{ClassName_mangled}}TypeSpec->addRegularMember( "{{FieldName}}", offsetof(type_to_add, {{FieldName}}), "{{FieldType}}");
)");

std::string stl_decl = std::string(R"( 
    dataTypeInator->addToDictionary("{{STLName}}", new SpecifiedSequenceDataType<{{STLName}}>("{{STLName}}"));
)");

std::map<std::string, std::string> template_dictionary {
    {"top", io_src},
    {"call_class_decl", call_class_decl},
    {"class_decl", class_decl},
    {"field_decl", field_decl},
    {"stl_decl", stl_decl}
};
