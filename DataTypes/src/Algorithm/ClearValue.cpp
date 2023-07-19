#include "Algorithm/ClearValue.hpp"

#include "Type/Types.hpp"
#include "Type/NormalStructMember.hpp"
#include "Value/Value.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/PointerValue.hpp"
#include "Value/StringValue.hpp"


/**
 * @brief Clear out the entire contents of a type.
 */
namespace ClearValue {

    ClearValueVisitor::ClearValueVisitor(void * address) {
        address_stack.push(address);
    }

    bool ClearValueVisitor::visitPrimitiveDataType(const PrimitiveDataType * node) {
        // Primitives are actually an abstract base for the SpecifiedPrimitive<T>  type
        // So we use the clearValue() interface from PrimitiveType

        node->clearValue(address_stack.top());
        return true;
    }

    bool ClearValueVisitor::visitStringType(const StringDataType * node) {
        std::string * str_ptr = (std::string *) address_stack.top();
        *str_ptr = "";
        return true;
    }

    bool ClearValueVisitor::visitCompositeType(const CompositeDataType * node) {
        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++) {
            NormalStructMember * member = *it;
            
            const DataType * member_subtype = member->getSubType();

            // Push address of this subtype
            address_stack.push(member->getAddress(address_stack.top()));

            // Go into member
            member_subtype->accept(this);

            // Remove member address from stack
            address_stack.pop();
        }

        // TODO: Do the same thing for statics?
        // Should work fine, but it will repeated a lot

        return true;
    }

    bool ClearValueVisitor::visitArrayType(const ArrayDataType * node) {
        const DataType * subtype = node->getSubType();
        for (int i = 0; i < node->getElementCount(); i++) {
            void * elemAddress = (char *) address_stack.top() + (i * subtype->getSize());

            // Push element onto stack
            address_stack.push(elemAddress);

            subtype->accept(this);

            // Remove element from stack
            address_stack.pop();
        }

        return true;
    }

    bool ClearValueVisitor::visitPointerType(const PointerDataType * node) {
        void * addr_to_clear = address_stack.top();

        *(void**)addr_to_clear =  NULL;

        return true;
    }

    bool ClearValueVisitor::visitEnumeratedType(const EnumDataType * node) {
        node->clearValue(address_stack.top());
        return true;
    }

}