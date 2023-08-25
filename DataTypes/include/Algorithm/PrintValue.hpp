#include <iostream>
#include <stack>

#include "Algorithm/DataTypeVisitor.hpp"

namespace PrintValue {

    class PrintValueVisitor : public DataTypeVisitor {

        public:
            PrintValueVisitor(std::ostream &s, void *address);

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
            std::ostream& s;
            std::stack<void *> address_stack;
    };
}