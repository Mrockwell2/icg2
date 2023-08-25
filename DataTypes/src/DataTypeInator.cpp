#include "DataTypeInator.hpp"
#include "Type/ArrayDataType.hpp"
#include "Type/PointerDataType.hpp"
#include "Utils/MutableDeclaration.hpp"
#include "TypeDictionary.hpp"

DataTypeInator::DataTypeInator () : typeDictionary(new TypeDictionary) {
    // Probably shouldn't do this.
    typeDictionary->addBuiltinTypes();
}

DataTypeInator::DataTypeInator (TypeDictionary * dict) : typeDictionary(dict) {}

std::shared_ptr<const DataType> DataTypeInator::resolve(std::string name) {

    MutableDeclaration decl(name);

    std::string lookup_type_name = decl.getAbstractDeclarator();
    std::shared_ptr<const DataType> initial_lookup = typeDictionary->lookup(lookup_type_name);

    // If we find anything, return it.
    // If we don't find anything, but it's an array/ptr, make it and return that.
    // If we don't find anything and it's not an array/ptr, return null.
    if (decl.getNumberOfDimensions() == 0 || initial_lookup != NULL) {
        return initial_lookup;
    } else {
        // Create a new array/ptr value, make sure it gets added to dictionary

        // Pop off the last dimension, make an array/pointer, let the nested recursive structure do the rest

        // Pop first elem
        int this_dim = decl.popDimension();

        // Make a new declaration string for the subtype
        std::string next_level_decl = decl.getDeclarator(decl.getVariableName());

        DataType * new_type;

        if (this_dim == -1) {
            // Pointer case
            new_type = new PointerDataType(next_level_decl);
        } else {
            // Constrained dim case
            new_type = new ArrayDataType(next_level_decl, this_dim);

        }

        if (!new_type->validate(this)) {
            std::cerr << "Validate failed for " << new_type->toString() << std::endl;
            return std::shared_ptr<const DataType>(nullptr);
        }

        // Add to dictionary
        addToDictionary(lookup_type_name, new_type);

        return typeDictionary->lookup(lookup_type_name);
    }
}

void DataTypeInator::addToDictionary(std::string name, DataType * typeSpec) {
    typeDictionary->addTypeDefinition(name, typeSpec);
}

bool DataTypeInator::validateDictionary() {
    return typeDictionary->validate(this);
}


