#pragma once

#include "FindLeaves.hpp"
#include "FindDeps.hpp"
#include "ContainsPointer.hpp"
#include "LookupAddressAndTypeByName.hpp"
#include "LookupNameByAddressAndType.hpp"
#include "ContainsPointer.hpp"
#include "PrintValue.hpp"
#include "AssignValue.hpp"
#include "GetValue.hpp"
#include "ClearValue.hpp"

namespace DataTypeAlgorithm {

    /**
     * @brief Determine whether or not this type is a pointer, or has any pointer members or subtypes
     * 
     * @param node DataType to search
     * @return bool true if some subtype of this type is a pointer, false otherwise
     */
    ContainsPointer::Result containsPointer(const DataType * node);

    /**
     * @brief Find all of the leaves in this allocation as name-value pairs
     * 
     * @param node DataType to search
     * @param starting_name name of allocation that we're searching
     * @param starting_address starting address of allocation that we're searching
     * @return FindLeaves::Result 
     */
    FindLeaves::Result findLeaves(const DataType * node, std::string starting_name, void * starting_address);

    /**
     * @brief Find the addresses of all the dependencies this allocation has
     * 
     * @param node DataType to search
     * @param starting_address starting address of allocation that we're searching
     * @return FindLeaves::Result 
     */
    FindDependencies::Result findDependencies(const DataType * node, void * starting_address);


    /**
     * @brief Search in the allocation of the given type for a nested member with the correct name. If the name does not match the type, returns an empty result.
     * 
     * @param node DataType to search
     * @param starting_address address of allocation to search
     * @param full_name name of variable to match
     * @return LookupAddressAndTypeByName::Result 
     */
    LookupAddressAndTypeByName::Result lookupAddressAndTypeByName (const DataType * node, void * starting_address, std::string full_name);

    /**
     * @brief Search in the allocation of the given type for the address and type of a nested member with the correct name. If the name does not match the type, returns an empty result.
     * 
     * @param node DataType to search
     * @param starting_address address of allocation to search
     * @param name_elems name of variable to match, already parsed
     * @return LookupAddressAndTypeByName::Result 
     */
    LookupAddressAndTypeByName::Result lookupAddressAndTypeByName (const DataType * node, void * starting_address, MutableVariableName name_elems);

    /**
     * @brief Traverse the allocation of the given type to get the name of nested member that starts at the given address and is of the given type.
     * 
     * @param node DataType to search
     * @param starting_name Name of the root allocation
     * @param starting_address address of the allocation to search
     * @param lookup_address address of the variable we are looking for
     * @param search_type type of the variable we are looking for
     * @return LookupNamebyAddressAndType::Result 
     */
    LookupNameByAddressAndType::Result lookupNameByAddressAndType (const DataType * node, std::string starting_name, void * starting_address, void * lookup_address, const DataType * const search_type);

    /**
     * @brief Print an ascii representation of the value starting at the given address of the given type to the given stream
     * 
     * @param node DataType of value to print
     * @param s ostream to print to
     * @param address Starting address of value to print
     */
    void printValue(const DataType * node, std::ostream& s, void * address);

    /**
     * @brief Assign the value of a leaf type (string, pointer, primitive, enum)
     * 
     * @param node Type to assign to
     * @param val Value to assign, must be compatible with node
     * @param address instance of type to assign to
     * @return AssignValue::Result bool representing success
     */
    AssignValue::Result assignValue(const DataType * node, Value * val, void * address);

    /**
     * @brief Get the value of a leaf type (string, pointer, primitive, enum)
     * 
     * @param node Type to get from 
     * @param address instance of type to assign to
     * @return GetValue::Result Value * representing the value
     */
    GetValue::Result getValue(const DataType * node, void * address);

    /**
     * @brief Clear the contents of an instance of a type
     * 
     * @param node Type to clear
     * @param address instance of type to clear
     */
    void clearValue(const DataType * node, void * address);
}