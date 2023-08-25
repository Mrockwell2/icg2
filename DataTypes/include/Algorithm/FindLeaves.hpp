#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

#include <vector>
#include <stack>


namespace FindLeaves {

    // Struct for returning results
    struct Leaf {
        Leaf(MutableVariableName n, Value * v) : Leaf(n, v, false, NULL) {}
        Leaf(MutableVariableName n, Value * v, bool is_ptr, std::shared_ptr<const DataType> ptr_sub) : 
            name_stack(n), value(v), is_pointer(is_ptr), pointer_subtype(ptr_sub), is_stl(false), stl_size(0) {}

        MutableVariableName name_stack;
        Value * value;
        bool is_pointer;
        std::shared_ptr<const DataType> pointer_subtype;
        bool is_stl;
        int stl_size;
    };

    typedef std::vector<Leaf> Result;
            
    class FindLeavesVisitor : public DataTypeVisitor {

        public:
            FindLeavesVisitor();
            FindLeavesVisitor(std::string starting_name, void * starting_address);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) override;
            virtual bool visitCompositeType(std::shared_ptr<const CompositeDataType> node) override;
            virtual bool visitArrayType(std::shared_ptr<const ArrayDataType> node) override;
            virtual bool visitPointerType(std::shared_ptr<const PointerDataType> node) override;
            virtual bool visitEnumeratedType(std::shared_ptr<const EnumDataType> node) override;
            virtual bool visitStringType (std::shared_ptr<const StringDataType> node) override;
            virtual bool visitSequenceType (std::shared_ptr<const SequenceDataType>  node) override;

            // FindLeavesVisitor Specific Interface
            Result getResult();

        private:
            // Visitor State
            MutableVariableName current_name_stack;
            std::stack<void *> address_stack;

            // Result
            std::vector<Leaf> leaves;
    };

}
