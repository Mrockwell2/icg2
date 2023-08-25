#include <cassert>

#include "Algorithm/ResizeSequence.hpp"

#include "Type/VisitableTypes.hpp"
#include "Type/NormalStructMember.hpp"

namespace ResizeSequence {

    ResizeSequenceVisitor::ResizeSequenceVisitor(void * starting_address, std::string full_name, int num_elems) 
            : current_search_address(starting_address), name_elems(full_name), num_elems(num_elems) {}

    // Look for the matching address

    bool ResizeSequenceVisitor::visitCompositeType(std::shared_ptr<const CompositeDataType> node) {
        if (name_elems.empty()) {
            // Got to the end of the name, but we're at not at a sequence :/            
            std::cerr << "Tried to use ResizeSequence on something that is not a sequence." << std::endl;
            return false;
        }

        std::string next_elem = name_elems.pop_front();

        // If the next name elem is an index, we done messed up
        if (next_elem[0] == '[') {
            std::cerr << "Got an index where a name was expected." << std::endl;
            return false;
        }


        // Find the next step
        std::shared_ptr<const DataType> next_type = NULL;
        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++) {
            NormalStructMember * member = *it;
            if (member->getName() == next_elem) {
                // Found the next step!
                // Find the address of the next member
                current_search_address = member->getAddress(current_search_address);
                next_type = member->getSubType();
                break;
            }
        }

        // Only search the statics if we haven't found matching name yet
        if (next_type == NULL) {
            for (auto it = node->getStaticMemberListBegin(); it != node->getStaticMemberListEnd(); it++) {
                StaticStructMember * member = *it;
                if (member->getName() == next_elem) {
                    // Found the next step!
                    // Find the address of the next member
                    current_search_address = member->getAddress();
                    next_type = member->getSubType();

                    break;
                }
            }
        }

        if (next_type != NULL) {
            return next_type->accept(this);
        }

        std::cerr << "Could not find member named " << next_elem << " in type " << node->toString() << std::endl;
        return false;
    }

    bool ResizeSequenceVisitor::visitArrayType(std::shared_ptr<const ArrayDataType> node) {
        if (name_elems.empty()) {
            // Got to the end of the name, but we're at not at a sequence :/            
            std::cerr << "Tried to use ResizeSequence on something that is not a sequence." << std::endl;
            return false;
        }


        std::string next_elem = name_elems.pop_front();

        // If the next name elem is not an index, we done messed up
        if (next_elem[0] != '[') {
            std::cerr << "Got a name where an index was expected." << std::endl;
            return false;
        }

        // Trim off the brackets and parse as int
        size_t index = stoi (next_elem.substr(1, next_elem.size()-1));
        if (index >= node->getElementCount()) {
            std::cerr << "Index " << index << " requested, but type " << node->toString() << " only has " << node->getElementCount() << " elements." << std::endl;
            return false;
        }

        std::shared_ptr<const DataType> subType = node->getSubType();

        // Set the next search address
        // Just pretend our current_search_address isn't void for a sec
        current_search_address = (char *)current_search_address + subType->getSize() * index;
        return subType->accept(this);
    }

    bool ResizeSequenceVisitor::visitPointerType(std::shared_ptr<const PointerDataType> node) {
        return visitLeaf(node);
    }

    bool ResizeSequenceVisitor::visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) {
        return visitLeaf(node);
    }

    bool ResizeSequenceVisitor::visitEnumeratedType(std::shared_ptr<const EnumDataType> node) {
        return visitLeaf(node);
    }

    bool ResizeSequenceVisitor::visitStringType(std::shared_ptr<const StringDataType> node) {
        return visitLeaf(node);
    }

    bool ResizeSequenceVisitor::visitSequenceType (std::shared_ptr<const SequenceDataType>  node) {
        if (name_elems.empty() || (name_elems.size() == 1 && name_elems.front() == "size")) {
            // Yay we found our result!

            return node->resize(current_search_address, num_elems);
        }

        // Otherwise we could be referring to a nested sequence, keep looking

        std::string next_elem = name_elems.pop_front();

        // If the next name elem is not an index, we done messed up
        if (next_elem[0] != '[') {
            std::cerr << "Got a name where an index was expected." << std::endl;
            return false;
        }

        // Trim off the brackets and parse as int
        size_t index = stoi (next_elem.substr(1, next_elem.size()-1));
        if (index >= node->getNumElements(current_search_address)) {
            std::cerr << "Index " << index << " requested, but this object of type " << node->toString() 
                      << " only has " << node->getNumElements(current_search_address) << " elements." << std::endl;

            return false;
        }

        auto elem_addresses = node->getElementAddresses(current_search_address);
        current_search_address = elem_addresses[index];

        std::shared_ptr<const DataType> subType = node->getSubType();
        return subType->accept(this);
    }

    bool ResizeSequenceVisitor::visitLeaf(std::shared_ptr<const DataType> node) {
        // We're at a leaf. We should basically just not be here at all.

        if (name_elems.empty()) {
            // Got to the end of the name, but we're at not at a sequence :/            
            std::cerr << "Tried to use ResizeSequence on something that is not a sequence." << std::endl;
        } else {
            std::cerr << "At a leaf type, but there are still name elements left to find." << std::endl;
        }

        return false;

    }
}