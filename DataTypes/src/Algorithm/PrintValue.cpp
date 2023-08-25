#include <algorithm>

#include "Algorithm/PrintValue.hpp"
#include "Type/VisitableTypes.hpp"
#include "Type/NormalStructMember.hpp"

namespace PrintValue {

    PrintValueVisitor::PrintValueVisitor(std::ostream &s, void *address) : s(s) {
        address_stack.push(address);
    }

    bool PrintValueVisitor::visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) {
        node->printValue(s, address_stack.top());
        return true;
    }

    bool PrintValueVisitor::visitCompositeType(std::shared_ptr<const CompositeDataType> node) {
        s << "{";
        int counter = 0;

        // Normal members
        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++, counter++) {
            if (counter != 0) {
                s << ", ";
            }
            
            NormalStructMember * member = *it;
            std::shared_ptr<const DataType> member_subtype = member->getSubType();
            address_stack.push(member->getAddress(address_stack.top()));

            // Go into member
            member_subtype->accept(this);

            // Remove member name from stack
            address_stack.pop();
        }
        s << "}";

        return true;
    }

    bool PrintValueVisitor::visitArrayType(std::shared_ptr<const ArrayDataType> node) {
        s << "[";

        for (unsigned int i=0; i < node->getElementCount() ; i++) {
            if (i) {
                s << ", ";
            }

            std::shared_ptr<const DataType> subType = node->getSubType();

            address_stack.push( (char*) address_stack.top() + (i * subType->getSize()) );
            subType->accept( this );
            address_stack.pop();
        }
        s << "]";

        return true;
    }

    bool PrintValueVisitor::visitPointerType(std::shared_ptr<const PointerDataType> node) {
        s << *(void**)address_stack.top();

        return true;
    }

    bool PrintValueVisitor::visitEnumeratedType(std::shared_ptr<const EnumDataType> node) {
        int enum_val = *(int*) address_stack.top();
        s << node->lookupEnumeratorName(enum_val);

        return true;
    }

    bool PrintValueVisitor::visitStringType (std::shared_ptr<const StringDataType> node) {
        s << "\"" <<  * (std::string * ) address_stack.top() << "\"";

        return true;
    }

    bool PrintValueVisitor::visitSequenceType (std::shared_ptr<const SequenceDataType>  node) {
        std::shared_ptr<const DataType> subType = node->getSubType();
        auto elem_addresses = node->getElementAddresses(address_stack.top());

        s << "[";

        for (int i = 0; i < elem_addresses.size(); i++) {
            if (i != 0) {
                s << ", ";
            }

            address_stack.push( elem_addresses[i] );
            subType->accept( this );
            address_stack.pop();
        }

        s << "]";

        return true;
    }

}