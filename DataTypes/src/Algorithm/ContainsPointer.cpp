#include "Algorithm/ContainsPointer.hpp"

#include "Type/VisitableTypes.hpp"
#include "Type/NormalStructMember.hpp"

namespace ContainsPointer {

    ContainsPointerVisitor::ContainsPointerVisitor() {}

    bool ContainsPointerVisitor::visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) {
        return false;
    }

    bool ContainsPointerVisitor::visitStringType(std::shared_ptr<const StringDataType> node) {
        return false;
    }

    bool ContainsPointerVisitor::visitCompositeType(std::shared_ptr<const CompositeDataType> node) {
        bool result = false;

        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++) {
            NormalStructMember * member = *it;
            std::shared_ptr<const DataType> member_subtype = member->getSubType();
            
            // Go into member
            result |= member_subtype->accept(this);
            
        }

        return result;
    }

    bool ContainsPointerVisitor::visitArrayType(std::shared_ptr<const ArrayDataType> node) {
        std::shared_ptr<const DataType> subtype = node->getSubType();
        return subtype->accept(this);
    }

    bool ContainsPointerVisitor::visitPointerType(std::shared_ptr<const PointerDataType> node) {
        // This is a case that we're looking for!
        return true;
    }

    bool ContainsPointerVisitor::visitEnumeratedType(std::shared_ptr<const EnumDataType> node) {
        return false;
    }

    bool ContainsPointerVisitor::visitSequenceType (std::shared_ptr<const SequenceDataType>  node) {
        throw std::logic_error("Sequence type not implemented in ContainsPointerVisitor");
    }

}