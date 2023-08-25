#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Utils/MutableVariableName.hpp"


namespace LookupNameByAddressAndType {

    struct Result {
        Result () : success(false) {}

        bool success;
        std::string name;
    };

    class LookupNameByAddressVisitor : public DataTypeVisitor {

        public:
            LookupNameByAddressVisitor(std::string starting_name, void * starting_address, void * lookup_address, std::shared_ptr<const DataType> const search_type);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) override;
            virtual bool visitCompositeType(std::shared_ptr<const CompositeDataType> node) override;
            virtual bool visitArrayType(std::shared_ptr<const ArrayDataType> node) override;
            virtual bool visitPointerType(std::shared_ptr<const PointerDataType> node) override;
            virtual bool visitEnumeratedType(std::shared_ptr<const EnumDataType> node) override;
            virtual bool visitStringType (std::shared_ptr<const StringDataType> node) override;
            virtual bool visitSequenceType (std::shared_ptr<const SequenceDataType>  node) override;

            // LookupAddress Interface
            std::string getResult();


        private:
            // Visitor State
            
            // We don't actually care about the absolute address, only the offset within the type
            long search_offset;

            // Const all around
            std::shared_ptr<const DataType> const search_type;

            // Result variable
            MutableVariableName name_stack;

            // Helper
            bool typeCheck(std::shared_ptr<const DataType> node);
            bool visitLeaf(std::shared_ptr<const DataType> node);
    };
}