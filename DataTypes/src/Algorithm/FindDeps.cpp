#include "Algorithm/FindDeps.hpp"

#include "Type/Types.hpp"
#include "Type/NormalStructMember.hpp"

#include "Value/PointerValue.hpp"

#include "Algorithm/DataTypeAlgorithm.hpp"

namespace FindDependencies {

    FindDepsVisitor::FindDepsVisitor() {}

    FindDepsVisitor::FindDepsVisitor(void * starting_address) {
        address_stack.push(starting_address);
    }

    // Harvest all the values from the tree

    bool FindDepsVisitor::visitPrimitiveDataType(const PrimitiveDataType * node) {
        return false;
    }

    bool FindDepsVisitor::visitStringType(const StringDataType * node) {
        return false;
    }

    bool FindDepsVisitor::visitCompositeType(const CompositeDataType * node) {
		deps.emplace_back(address_stack.top());

        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++) {
            NormalStructMember * member = *it;
            
            const DataType * member_subtype = member->getSubType();
            
            address_stack.push(member->getAddress(address_stack.top()));

            // Go into member
            member_subtype->accept(this);

            // Remove member name from stack
            address_stack.pop();
        }

		return true;
    }

    bool FindDepsVisitor::visitArrayType(const ArrayDataType * node) {
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

    bool FindDepsVisitor::visitPointerType(const PointerDataType * node) {
		Value * val = DataTypeAlgorithm::getValue(node, address_stack.top());
    	PointerValue * pointer_val = dynamic_cast<PointerValue *> (val);
		void * ptr = pointer_val->getPointer();
		if(ptr) {
			deps.emplace_back(ptr);
        	return true;
		}
		return false;
    }

    bool FindDepsVisitor::visitEnumeratedType(const EnumDataType * node) {
        return false;
    }

    Result FindDepsVisitor::getResult() {
        return deps;
    }
}