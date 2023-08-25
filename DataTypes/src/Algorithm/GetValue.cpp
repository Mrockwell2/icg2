#include "Algorithm/GetValue.hpp"

#include "Type/VisitableTypes.hpp"
#include "Type/NormalStructMember.hpp"
#include "Value/Value.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/PointerValue.hpp"
#include "Value/StringValue.hpp"


/**
 * @brief get a leaf value.
 * @note I don't think we should support CompositeValue, ArrayValue, and other possible future multi-value containers.
 * I think we should only support "leaf" types - Primitive, String, Pointer, Enum
 * And leave it up to other algorithms to find this types
 * This is already what J_Checkpoint agent does
 * However, if we switch to another format of checkpoint, like json or xml, which would lend itself to reconstructing entire objects rather than just leaves.
 * This might be annoying if we decide to consolidate arrays or something
 * idk maybe not
 * It's simple enough to move the functionality here, it's probably better to keep the support rather than drop that.
 * Actually no. I think tying the 'Value' hierarchy directly to the 'DataTypes' hierarchy is a bad idea. I want to drop that.
 * If we want to create whole values in the future, we can make another DataTypeAlgorithm for that. But this isn't what I want to do.
 * I also really hate that we're dynamic casting all over the place
 * But I don't see a way around it without ~really~ tying the Value and Type hierarchies together
 */
namespace GetValue {

    GetValueVisitor::GetValueVisitor(void * address) : address(address), result(NULL) {}

    bool GetValueVisitor::visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) {
        // Primitives are actually an abstract base for the SpecifiedPrimitive<T>  type
        // So we use the GetValue() interface from PrimitiveType

        result = node->getValue(address);
        return true;
    }

    bool GetValueVisitor::visitStringType(std::shared_ptr<const StringDataType> node) {
        result = new StringValue(* (std::string * ) address);
        return true;
    }

    bool GetValueVisitor::visitCompositeType(std::shared_ptr<const CompositeDataType> node) {
        std::cerr << "Must get a leaf type" << std::endl;
        return false;
    }

    bool GetValueVisitor::visitArrayType(std::shared_ptr<const ArrayDataType> node) {
        std::cerr << "Must get a leaf type" << std::endl;
        return false;
    }

    bool GetValueVisitor::visitPointerType(std::shared_ptr<const PointerDataType> node) {
        result = new PointerValue(*(void**)address);
        return true;
    }

    bool GetValueVisitor::visitEnumeratedType(std::shared_ptr<const EnumDataType> node) {
        result = new IntegerValue(node->getValue(address));
        return true;
    }

    bool GetValueVisitor::visitSequenceType (std::shared_ptr<const SequenceDataType>  node) {
        std::cerr << "Must get a leaf type" << std::endl;
        return false;    
    }

    Result GetValueVisitor::getResult () {
        return result;
    }

}