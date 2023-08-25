#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

#include <vector>
#include <stack>


namespace ResizeSequence {
            
    class ResizeSequenceVisitor : public DataTypeVisitor {

        public:
            ResizeSequenceVisitor(void * address, std::string variable_name, int num_elems);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) override;
            virtual bool visitCompositeType(std::shared_ptr<const CompositeDataType> node) override;
            virtual bool visitArrayType(std::shared_ptr<const ArrayDataType> node) override;
            virtual bool visitPointerType(std::shared_ptr<const PointerDataType> node) override;
            virtual bool visitEnumeratedType(std::shared_ptr<const EnumDataType> node) override;
            virtual bool visitStringType (std::shared_ptr<const StringDataType> node) override;
            virtual bool visitSequenceType (std::shared_ptr<const SequenceDataType>  node) override;

        private:

            bool visitLeaf(std::shared_ptr<const DataType> node);

            // Visitor Intermediate State
            void * current_search_address;
            MutableVariableName name_elems;
            int num_elems;
    };
}
