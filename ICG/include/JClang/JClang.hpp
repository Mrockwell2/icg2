#pragma once


#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace JClang {

    enum NodeKind {
        ClassDecl,
        FieldDecl,
        NamespaceDecl,
        ClassTemplateDecl,
        ClassTemplateSpecialization,
        TemplateParameterDecl,
        TemplateArgument,
        Unknown
    };

    /**
     * @brief Get the kind of a node
     * 
     * @param node node of json AST
     * @return NodeKind enum of node type
     */
    NodeKind getNodeKind (json& node);

    /**
     * @brief Get the Qualified Type of the field 
     * 
     * @param field_node json node that represents a field declaration
     * @return std::string fully qualified type of field
     */
    std::string getQualifiedTypeOfField (json& field_node);
}