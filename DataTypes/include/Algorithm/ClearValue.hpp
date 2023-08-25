#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include <stack>


namespace ClearValue {

    typedef bool Result;
            
    class ClearValueVisitor : public DataTypeVisitor {

        public:
            ClearValueVisitor(void * address);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) override;
            virtual bool visitCompositeType(std::shared_ptr<const CompositeDataType> node) override;
            virtual bool visitArrayType(std::shared_ptr<const ArrayDataType> node) override;
            virtual bool visitPointerType(std::shared_ptr<const PointerDataType> node) override;
            virtual bool visitEnumeratedType(std::shared_ptr<const EnumDataType> node) override;
            virtual bool visitStringType (std::shared_ptr<const StringDataType> node) override;
            virtual bool visitSequenceType (std::shared_ptr<const SequenceDataType>  node) override;

        private:
            // Visitor State
            std::stack<void *> address_stack;
    };
}
