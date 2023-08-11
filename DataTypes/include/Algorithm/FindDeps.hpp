#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

#include <vector>
#include <stack>


namespace FindDependencies {

    typedef std::vector<void *> Result;
            
    class FindDepsVisitor : public DataTypeVisitor {

        public:
            FindDepsVisitor();
            FindDepsVisitor(void * starting_address);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;

            // FindDepsVisitor Specific Interface
            Result getResult();

        private:
            // Visitor State
            std::stack<void *> address_stack;

            // Result
            std::vector<void *> deps;
    };

}
