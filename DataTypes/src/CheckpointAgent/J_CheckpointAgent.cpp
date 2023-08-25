#include "CheckpointAgent/J_CheckpointAgent.hpp"

#include "Utils/ParsedAssignment.hpp"
#include "Utils/ParsedDeclaration.hpp"
#include "Type/DataType.hpp"
#include "Value/PointerValue.hpp"
#include "Value/StringValue.hpp"
#include "Value/IntegerValue.hpp"
#include "Algorithm/DataTypeAlgorithm.hpp"

const std::string J_CheckpointAgent::error_str = "<UNDEFINED>";
const std::string J_CheckpointAgent::resize_command = "RESIZE_STL";


J_CheckpointAgent::J_CheckpointAgent(DataTypeInator * inator) : dataTypeInator(inator) {}


/* ==================================================================== */
/*                          DUMP CHECKPOINT                             */
/* ==================================================================== */

int J_CheckpointAgent::dump ( std::ostream& checkpoint_out, std::vector<AllocInfo *> allocations_to_checkpoint) {
    
    // First go through and check that the allocations are well formed
    auto it = allocations_to_checkpoint.begin();
    while(it != allocations_to_checkpoint.end()) {
        if((*it)->getDataType() == NULL) {
            std::cerr << "Alloc info named " << getCheckpointableName(*it) << " does not have a data type associated with it. Cannot checkpoint." << std::endl;
            it = allocations_to_checkpoint.erase(it);
        }
        else {
            ++it;
        }
    }

    // Write out declarations
    checkpoint_out << "// Variable Declarations." << std::endl;
    checkpoint_out.flush();

    for (AllocInfo * allocInfo : allocations_to_checkpoint) {
        StorageClass::e storageClass = allocInfo->getStorageClass();
        // Only write the declaration if it's a local alloc
        if ( storageClass == StorageClass::LOCAL) {
            writeDeclaration(checkpoint_out, allocInfo);
        }
    }

    // Dump the contents of each of the local and extern allocations.
    checkpoint_out << std::endl << std::endl << "// Variable Assignments." << std::endl;
    checkpoint_out.flush();

    for (AllocInfo * allocInfo : allocations_to_checkpoint) {
        writeAssignment(checkpoint_out, allocInfo, allocations_to_checkpoint);
    }

    return 0;
}

bool J_CheckpointAgent::writeDeclaration( std::ostream& checkpoint_out, const AllocInfo * allocInfo) {
    std::string name = getCheckpointableName(allocInfo);

    // Write the declaration
    checkpoint_out << allocInfo->getDataType()->makeDeclaration(name) << " ;\n";

    return true;
}

bool J_CheckpointAgent::writeAssignment( std::ostream& checkpoint_out, const AllocInfo * alloc_to_write, const std::vector<AllocInfo *>& additional_search_allocs) {
    std::string name = getCheckpointableName(alloc_to_write);

    FindLeaves::Result result = DataTypeAlgorithm::findLeaves(alloc_to_write->getDataType(), name, alloc_to_write->getStart());

    // This is all of the leaf values in this alloc info
    // Write out an assignment for each
    for (auto leaf : result) {
        std::string value;
        if (leaf.is_pointer) {
            // If it's a pointer, we have to resolve the value to the name of another variable in the set of allocations.
            // Do a sketch cast but it's ok because we know that Value is definitely a PointerValue
            // This is bad design >:(
            void * ptr_value = ((PointerValue *) leaf.value )->getPointer();
            std::string ptr_name = resolvePointer(ptr_value, leaf.pointer_subtype, additional_search_allocs);
            if (ptr_name == error_str) {
                std::cerr << "Warning: Pointer " << leaf.name_stack.toString() << " was not found in managed memory. Skipping." << std::endl;
                continue;
            }

            value = "&" + ptr_name;

        } else if (leaf.is_stl) {
            checkpoint_out << resize_command << " ";
            value = std::to_string(leaf.stl_size);
        } else if (leaf.value->getValueType() == Value::ValueType::STRING) {
            StringValue * str_val = static_cast<StringValue *> (leaf.value);
            value = str_val->getEscapedString();
        } else {
            value = leaf.value->toString();
        }
        checkpoint_out << leaf.name_stack.toString() << " = " << value << " ;" << std::endl;
    }

    return true;
}


std::string J_CheckpointAgent::resolvePointer(void * ptr_to_resolve, std::shared_ptr<const DataType> expected_type, const std::vector<AllocInfo *>& allocs_to_search) {
    for (auto allocInfo : allocs_to_search) {
        if (allocInfo->contains(ptr_to_resolve)) {

            auto result = DataTypeAlgorithm::lookupNameByAddressAndType(allocInfo->getDataType(), getCheckpointableName(allocInfo), 
                                                                        allocInfo->getStart(), ptr_to_resolve, expected_type);
            
            // If something went wrong, just continue.
            // Although we should probably be able to just return an error
            // assuming that the AllocInfos never overlap, which ~should~ be true
            if (!result.success) {
                continue;
            }

            return result.name;
        }
    }

    // Oh no we didn't find it :(
    return error_str;
}

std::string J_CheckpointAgent::getCheckpointableName(const AllocInfo * allocInfo) {
    std::string name = allocInfo->getName();

    if ( name.empty() ) {
        name = allocInfo->getSerialName();
    }

    return name;
}

/* ==================================================================== */
/*                          RESTORE CHECKPOINT                          */
/* ==================================================================== */

std::vector<AllocInfo *> J_CheckpointAgent::restore( std::istream& checkpoint_in, std::vector<AllocInfo *> allocations_to_restore) {
    
    std::string checkpoint_line;
    while (std::getline(checkpoint_in, checkpoint_line)) {
        if (checkpoint_line.size() == 0) continue;
        
        // Ignore anything that begins with "//"
        if (checkpoint_line.find("//") == 0) continue;
        if (checkpoint_line.find("clear_all") == 0) continue;  // TODO: DONT DO THIS

        // Handle special commands
        if (checkpoint_line.find(resize_command) != std::string::npos ) {
            handleResizeCommand (checkpoint_line, allocations_to_restore);
        } else {
            // If a line has an "=" it's an assignment. If it doesn't, it's a declaration.
            if (checkpoint_line.find("=") == std::string::npos) {
                // Parse this line as a declaration
                try {
                    allocations_to_restore.push_back(restoreDeclaration(checkpoint_line));
                } catch (std::exception& e) {
                    std::cerr << "J_CheckpointAgent: An error occurred while processing declaration \"" << checkpoint_line << "\"\n\tMessage: " << e.what() << std::endl;
                }

            } else {
                // Parse this line as an assignment
                try {
                    restoreAssignment(checkpoint_line, allocations_to_restore);
                } catch (std::exception& e) {
                    // TODO: BETTER ERROR AND WARNING SYSTEM
                    std::cerr << "J_CheckpointAgent: An error occurred while processing assignment \"" << checkpoint_line << "\"\n\tMessage: " << e.what() << std::endl;
                }
            }
        }
    }

    return allocations_to_restore;
}

AllocInfo * J_CheckpointAgent::findAllocByName(std::string name, std::vector<AllocInfo *> search_allocs) {
    for (auto alloc_ptr : search_allocs) {
        if (getCheckpointableName(alloc_ptr) == name)
            return alloc_ptr;
    }

    return NULL;
}

void *  J_CheckpointAgent::lookupPointer(std::string varname, const std::vector<AllocInfo *>& allocs_to_search) {
    MutableVariableName var_elems (varname);

    std::string root_name = var_elems.pop_front();
    AllocInfo * alloc_to_assign = findAllocByName(root_name, allocs_to_search);
    if (alloc_to_assign == NULL) {
        return NULL;
    }
    
    // Find the address that we're assigning to
    auto result = DataTypeAlgorithm::lookupAddressAndTypeByName(alloc_to_assign->getDataType(), alloc_to_assign->getStart(), var_elems);

    // auto result = visitor.getResult();
    return result.address;
}

AllocInfo * J_CheckpointAgent::restoreDeclaration(std::string declaration) {
    // Strip off the ;
    // Maybe the ParsedDeclaration needs to be updated to handle this.....

    size_t semicolon_pos = declaration.find(";");
    declaration = declaration.substr(0, semicolon_pos);
    ParsedDeclaration typeName(declaration);
    std::string abstractDeclarator = typeName.getTypeSpecifier();
    std::string variableName = typeName.getVariableName();


    AllocInfo * new_alloc = new AllocInfo(variableName, dataTypeInator->resolve(abstractDeclarator));
    return new_alloc;
}

bool J_CheckpointAgent::restoreAssignment(std::string assignment_string, const std::vector<AllocInfo *>& additional_search_allocs) {
    ParsedAssignment assignment(assignment_string);

    // Parsing this assignment gets us the variable name and creates the Value object
    std::string full_varname = assignment.getVariableName();
    Value * this_value = assignment.getValue();
    // TODO: this_value should probably be a unique_ptr

    MutableVariableName var_elems (full_varname);

    std::string root_name = var_elems.pop_front();
    AllocInfo * alloc_to_assign = findAllocByName(root_name, additional_search_allocs);
    if (alloc_to_assign == NULL) {
        throw std::logic_error("Could not find allocation named " + root_name);
    }

    // Find the address and the datatype that we're assigning to
    auto result = DataTypeAlgorithm::lookupAddressAndTypeByName(alloc_to_assign->getDataType(), alloc_to_assign->getStart(), var_elems);
    if (!result.success) {
        throw std::logic_error("Could not find variable named " + full_varname);
    }

    std::shared_ptr<const DataType> this_type = result.type;
    void * this_addr = result.address;

    // Special case - pointer :(
    if (result.isPointer ) {
        // this will return a string that says "&varname"
        // There's a better design than this
        // But we gotta handle special cases one way or another

        if (this_value->getValueType() != Value::ValueType::STRING) {
            throw std::logic_error(full_varname + " is a pointer, but the value assigned is not compatible.");
        }

        StringValue * ptr_name_value = static_cast<StringValue *> (this_value);
        if (ptr_name_value->getRawString().size() < 2 ||  ptr_name_value->getRawString().at(0) != '&' ) {
            throw std::logic_error(full_varname + " is a pointer, but the value assigned is not compatible.");
        }

        // Chop off the &
        std::string ptr_varname = ptr_name_value->getRawString().substr(1);
        void * raw_addr = lookupPointer(ptr_varname, additional_search_allocs);
        if (raw_addr == NULL) {
            throw std::logic_error("Could not find variable named " + full_varname);
        }

        delete this_value;
        this_value = new PointerValue(raw_addr);       
    }

    // Assign!!!!
    bool assign_status = DataTypeAlgorithm::assignValue(this_type, this_value, this_addr);

    // cleanup
    delete this_value;

    if (!assign_status) {
        throw std::logic_error("Mismatched types in assignment\"" + assignment_string + "\"");
    }

    return true;
}

void J_CheckpointAgent::handleResizeCommand (std::string command_str, const std::vector<AllocInfo *>& allocs_to_search) {
    // Strip off command name from front of string
    int cmd_position = command_str.find(resize_command);
    if (cmd_position != std::string::npos) {
        int assignment_start = cmd_position + resize_command.size();
        command_str = command_str.substr(assignment_start);
    }

    ParsedAssignment assignment(command_str);

    // Parsing this assignment gets us the variable name and creates the Value object
    std::string full_varname = assignment.getVariableName();
    Value * size_value = assignment.getValue();

    if (size_value->getValueType() != Value::ValueType::INTEGER) {
        std::cerr << "Invalid STL resize command: " << command_str << std::endl;
        return;
    }

    IntegerValue * size_value_int = static_cast<IntegerValue *> (size_value);
    int num_elems = size_value_int->getIntegerValue();

    MutableVariableName var_elems (full_varname);
    std::string root_name = var_elems.pop_front();
    AllocInfo * alloc_to_assign = findAllocByName(root_name, allocs_to_search);

    // If the root allocation doesn't exist, nothing to do
    if (alloc_to_assign == NULL) {
        std::cerr << "Could not find " << full_varname << " in memory managed allocations." << std::endl;
        return;
    }

    DataTypeAlgorithm::resizeSequence(alloc_to_assign->getDataType(), alloc_to_assign->getStart(), var_elems.toString(), num_elems);
}