#include "Algorithm/DataTypeAlgorithm.hpp"
#include "Type/DataType.hpp"

namespace DataTypeAlgorithm {

    void checkType(std::shared_ptr<const DataType> node, const char * function_name) {
        if (node == NULL) {
            std::string error_msg = std::string(function_name) + " cannot be run: DataType parameter is null";
            throw std::logic_error (error_msg);
        }

        if (! node->isValid()) {
            std::string error_msg = std::string(function_name) + " cannot be run: DataType parameter is unvalidated";
            throw std::logic_error (error_msg);
        }
    }    

    ContainsPointer::Result containsPointer(std::shared_ptr<const DataType> node) {
        checkType(node, __FUNCTION__);

        ContainsPointer::ContainsPointerVisitor visitor;
        return visitor.go(node);
    }

    FindLeaves::Result findLeaves(std::shared_ptr<const DataType> node, std::string starting_name, void * starting_address) {
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


    LookupAddressAndTypeByName::Result _lookupAddressAndTypeByName (std::shared_ptr<const DataType> node, LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor& visitor) {
        checkType(node, __FUNCTION__);

        bool status = visitor.go(node);
        if (!status) {
            LookupAddressAndTypeByName::Result empty_result;
            return empty_result;
        }

        return visitor.getResult();
    }

    LookupAddressAndTypeByName::Result lookupAddressAndTypeByName (std::shared_ptr<const DataType> node, void * starting_address, std::string full_name) {
        checkType(node, __FUNCTION__);

        LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor visitor(starting_address, full_name);
        return _lookupAddressAndTypeByName(node, visitor);
    }

    LookupAddressAndTypeByName::Result lookupAddressAndTypeByName (std::shared_ptr<const DataType> node, void * starting_address, MutableVariableName name_elems) {
        checkType(node, __FUNCTION__);

        LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor visitor(starting_address, name_elems);
        return _lookupAddressAndTypeByName(node, visitor);
    }

    LookupNameByAddressAndType::Result lookupNameByAddressAndType (std::shared_ptr<const DataType> node, std::string starting_name, void * starting_address, void * lookup_address, std::shared_ptr<const DataType> const search_type) {
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

    void printValue(std::shared_ptr<const DataType> node, std::ostream& s, void * address) {
        checkType(node, __FUNCTION__);
        
        PrintValue::PrintValueVisitor visitor(s, address);
        visitor.go(node);
    }

    AssignValue::Result assignValue(std::shared_ptr<const DataType> node, Value * val, void * address) {
        checkType (node, __FUNCTION__);

        AssignValue::AssignValueVisitor visitor(val, address);
        return visitor.go(node);
    }

    GetValue::Result getValue(std::shared_ptr<const DataType> node, void * address) {
        checkType (node, __FUNCTION__);

        GetValue::GetValueVisitor visitor(address);
        visitor.go(node);

        return visitor.getResult();
    }

    void clearValue(std::shared_ptr<const DataType> node, void * address) {
        checkType (node, __FUNCTION__);

        ClearValue::ClearValueVisitor visitor(address);
        visitor.go(node);
    }   


    bool resizeSequence(std::shared_ptr<const DataType> node, void * starting_address, std::string full_name, int num_elems) {
        checkType (node, __FUNCTION__);

        ResizeSequence::ResizeSequenceVisitor visitor(starting_address, full_name, num_elems);
        return visitor.go(node);   
    }
    
}