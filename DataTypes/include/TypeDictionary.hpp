#pragma once

#include <map>
#include <string>
#include <stdexcept>
#include <memory>

#include "Type/DataType.hpp"
#include "Utils/MutableDeclaration.hpp"

class DataTypeInator;

/**
 Stores name / typespecifier pairs.
 */
class TypeDictionary {

    public:

    TypeDictionary();

    /**
     Get the DataType of the for the typedef'ed name.
     */
    std::shared_ptr<const DataType> lookup(std::string typeName);

    /**
     Add a type definiton to the dictionary.
     */
    void addTypeDefinition(std::string name, DataType * typeSpec);


    /**
     */
    bool validate(DataTypeInator * dataTypeInator);

    /**
     Dump the entire TypeDictionary to a std::string.
     */
    std::string toString();

    /**
      Destructor.
     */
    ~TypeDictionary();

    // helper
    // This probably doesn't belong here
    void addBuiltinTypes();
    
    private:
    bool is_valid;

    std::map<std::string, std::shared_ptr<DataType>> typeDictionary;
    
    // Make these guys unique_ptrs
    // Or just objects? No reason to make this a pointer i think
    std::map<std::string, TypeDictionary *> namespaceDictionary;

    // Helpers to avoid having to parse the declaration multiple times
    void addTypeDefinition(MutableDeclaration& nameParts, DataType * typeSpec);
    std::shared_ptr<const DataType> lookup(MutableDeclaration& nameParts);
};
