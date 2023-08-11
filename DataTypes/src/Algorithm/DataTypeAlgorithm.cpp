#include "Algorithm/DataTypeAlgorithm.hpp"
#include "Type/DataType.hpp"

namespace DataTypeAlgorithm {

    void checkType(const DataType * node, const char * function_name) {
        if (node == NULL) {
            std::string error_msg = std::string(function_name) + " cannot be run: DataType parameter is null";
            throw std::logic_error (error_msg);
        }

        if (! node->isValid()) {
            std::string error_msg = std::string(function_name) + " cannot be run: DataType parameter is unvalidated";
            throw std::logic_error (error_msg);
        }
    }    

    ContainsPointer::Result containsPointer(const DataType * node) {
        checkType(node, __FUNCTION__);

        ContainsPointer::ContainsPointerVisitor visitor;
        return visitor.go(node);
    }

    FindLeaves::Result findLeaves(const DataType * node, std::string starting_name, void * starting_address) {
        checkType(node, __FUNCTION__);

        FindLeaves::FindLeavesVisitor visitor(starting_name, starting_address);
        bool status = visitor.go(node);
        
        if (!status) {
            FindLeaves::Result empty_result;
            return empty_result;
        }
        
        return visitor.getResult();
    }

    FindDependencies::Result findDependencies(const DataType * node, void * starting_address) {
        checkType(node, __FUNCTION__);

        FindDependencies::FindDepsVisitor visitor(starting_address);
        bool depFound = visitor.go(node);
        
        if (!depFound) {
            FindDependencies::Result default_result({starting_address});
            return default_result;
        }
        
        return visitor.getResult();
    }


    LookupAddressAndTypeByName::Result _lookupAddressAndTypeByName (const DataType * node, LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor& visitor) {
        checkType(node, __FUNCTION__);

        bool status = visitor.go(node);
        if (!status) {
            LookupAddressAndTypeByName::Result empty_result;
            return empty_result;
        }

        return visitor.getResult();
    }

    LookupAddressAndTypeByName::Result lookupAddressAndTypeByName (const DataType * node, void * starting_address, std::string full_name) {
        checkType(node, __FUNCTION__);

        LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor visitor(starting_address, full_name);
        return _lookupAddressAndTypeByName(node, visitor);
    }

    LookupAddressAndTypeByName::Result lookupAddressAndTypeByName (const DataType * node, void * starting_address, MutableVariableName name_elems) {
        checkType(node, __FUNCTION__);

        LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor visitor(starting_address, name_elems);
        return _lookupAddressAndTypeByName(node, visitor);
    }

    LookupNameByAddressAndType::Result lookupNameByAddressAndType (const DataType * node, std::string starting_name, void * starting_address, void * lookup_address, const DataType * const search_type) {
        checkType(node, __FUNCTION__);

        try {
            LookupNameByAddressAndType::LookupNameByAddressVisitor visitor(starting_name, starting_address, lookup_address, search_type);
            bool status = visitor.go(node);
            if (!status) {
                LookupNameByAddressAndType::Result empty_result;
                return empty_result;
            }

            LookupNameByAddressAndType::Result result;
            result.success = true;
            result.name = visitor.getResult();
            return result;

        } catch (std::exception& e) {
            std::cerr << "LookupNameByAddressVisitor threw an exception: " << e.what() << std::endl;
            return std::move(LookupNameByAddressAndType::Result());
        }
    }

    void printValue(const DataType * node, std::ostream& s, void * address) {
        checkType(node, __FUNCTION__);
        
        PrintValue::PrintValueVisitor visitor(s, address);
        visitor.go(node);
    }

    AssignValue::Result assignValue(const DataType * node, Value * val, void * address) {
        checkType (node, __FUNCTION__);

        AssignValue::AssignValueVisitor visitor(val, address);
        return visitor.go(node);
    }

    GetValue::Result getValue(const DataType * node, void * address) {
        checkType (node, __FUNCTION__);

        GetValue::GetValueVisitor visitor(address);
        visitor.go(node);

        return visitor.getResult();
    }

    void clearValue(const DataType * node, void * address) {
        checkType (node, __FUNCTION__);

        ClearValue::ClearValueVisitor visitor(address);
        visitor.go(node);
    }   
    
}