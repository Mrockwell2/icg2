#include <cassert>

#include "Algorithm/LookupNameByAddressAndType.hpp"

#include "Type/DataType.hpp"
#include "Type/CompositeDataType.hpp"
#include "Type/ArrayDataType.hpp"
#include "Type/PointerDataType.hpp"
#include "Type/EnumDataType.hpp"
#include "Type/StringDataType.hpp"
#include "Type/PrimitiveDataType.hpp"


#include "Type/NormalStructMember.hpp"

namespace LookupNameByAddressAndType {

    // LookupNameByAddressVisitor::LookupNameByAddressVisitor(std::string starting_name, void * starting_address, void * lookup_address) 
    //         : LookupNameByAddressVisitor(starting_name, starting_address, lookup_address, NULL) {}

    LookupNameByAddressVisitor::LookupNameByAddressVisitor(std::string starting_name, void * starting_address, void * lookup_address, std::shared_ptr<const DataType> const search_type)
            : search_type(search_type) {

        name_stack.pushName(starting_name);
        search_offset = (long) lookup_address - (long) starting_address;

        if (search_offset < 0) {
            throw std::logic_error("Lookup address is below starting address, cannot proceed.");
        }
    }

    // Look for the matching address

    bool LookupNameByAddressVisitor::visitCompositeType(std::shared_ptr<const CompositeDataType> node) {
        if (search_offset == 0) {
            // Address found!
            // Check the type and return
            if (typeCheck(node)) {
                return true;
            }

            // Otherwise, let it fall through below to check the first element
        }

        if (search_offset > node->getSize()) {
            std::cerr << "Something went wrong - search offset is larger than composite type.";
            std::cerr << "Current name stack: " << name_stack.toString() << "\tCurrent offset: " << search_offset << std::endl;
            return false;
        }

        // Look for the correct member
        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++) {
            const NormalStructMember * member = *it;
            
            std::shared_ptr<const DataType> member_subtype = member->getSubType();
            assert(member_subtype != NULL);

            int member_offset = member->getOffset();
            int member_size = member_subtype->getSize();

            if (search_offset >= member_offset && search_offset < member_offset + member_size) {
                // Found the correct member to go into!

                // Push the member name on stack
                name_stack.pushName(member->getName());
                search_offset = search_offset - member_offset;

                // Go into member
                return member_subtype->accept(this);
            }
            // just keep looking
        }

        // TODO: BITFIELD AND STATIC IS NOT IMPLEMENTED YET

        // Oh no!
        // If we didn't return already, we couldn't find the search offset
        return false;
    }

    bool LookupNameByAddressVisitor::visitArrayType(std::shared_ptr<const ArrayDataType> node) {
        // std::cout << "Visiting ArrayDataType with subtype " << node->getTypeSpecName() << std::endl;

        if (search_offset == 0) {
            // Address found!
            // However, it could be the array, or it could be something at the first element of the array.
            // Use the type to check.

            if (typeCheck(node)) {
                // it's this! yay!
                return true;
            } 

            // Otherwise check the first element
        }

        // If the offset is greater than the size of this array, there's an error.
        if (search_offset > node->getSize()) {
            std::cerr << "Search offset " << search_offset << " is greater than the size (" << node->getSize() << ") of this type (" << node->toString() << ")." << std::endl;
            return false;
        }

        std::shared_ptr<const DataType> subType = node->getSubType();

        // Figure out which element we should go into
        unsigned int elem_index = search_offset / subType->getSize();

        if (elem_index >= node->getElementCount()) {
            std::cerr << "Search offset indicates element " << elem_index << " of an array that only has " << node->getElementCount() << " elems." << std::endl;
            return false;
        }

        // New offset is the remainder of offset / subtypeSize
        search_offset = search_offset % subType->getSize();

        // Push the index onto the stack
        name_stack.pushIndex(elem_index);

        // Continue the search in the element
        return subType->accept(this);
    }

    bool LookupNameByAddressVisitor::visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) {
        return visitLeaf(node);
    }

    bool LookupNameByAddressVisitor::visitPointerType(std::shared_ptr<const PointerDataType> node) {
        // A pointer is a leaf type
        return visitLeaf(node);
    }

    bool LookupNameByAddressVisitor::visitEnumeratedType(std::shared_ptr<const EnumDataType> node) {
        // An enum is a leaf type
        return visitLeaf(node);
    }

    bool LookupNameByAddressVisitor::visitStringType (std::shared_ptr<const StringDataType> node) {
        return visitLeaf(node);
    }

    bool LookupNameByAddressVisitor::visitSequenceType (std::shared_ptr<const SequenceDataType>  node) {
        throw std::logic_error("Sequence type not implemented in LookupNameByAddressVisitor");
    }

    bool LookupNameByAddressVisitor::visitLeaf(std::shared_ptr<const DataType> node) {
        if (search_offset != 0) {
            std::cerr << "Got to a leaf, but the offset is not 0. Something went wrong. Current name stack: " << name_stack.toString() << "\tCurrent offset: " << search_offset << std::endl;
            return false;
        }

        // Yay! We found the variable!
        return typeCheck(node);
    }


    bool LookupNameByAddressVisitor::typeCheck(std::shared_ptr<const DataType> node) {
        // Check equality by comparing toString
        // This is not great but seems ok
        if (search_type != NULL && node->toString() != search_type->toString()) {
            // This is not necessarily an error, don't print anything
            return false;
        }        

        // Yay! We found it!
        return true;
    }

    std::string LookupNameByAddressVisitor::getResult() {
        return name_stack.toString();
    }

}