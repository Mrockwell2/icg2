#pragma once

#include <nlohmann/json.hpp>

#include "IntermediateRepresentation/ASTInfo.hpp"
#include "JClang/Scope.hpp"

using json = nlohmann::json;

namespace JClang {

    /**
     * @brief Traverse the AST and pull out relevant information
     * 
     * @param ast Top level AST
     * @return ASTInfo Information that we scraped out of it
     */
    ASTInfo traverseAST (json& ast);

    /**
     * @brief Process this node of the AST as a top level AST
     * 
     * @param ast_node node to process
     * @param scope current scope object
     * @return ASTInfo info scraped from this node and child nodes
     */
    ASTInfo scrape_ast (json& ast_node, Scope& scope);

    /**
     * @brief Process this node of the AST as a class declaration
     * 
     * @param class_node node to process
     * @param scope current scope object
     * @return ASTInfo info scraped from this node and child nodes
     */
    ASTInfo scrape_class_info (json& class_node, Scope& scope);

    /**
     * @brief Process this node of the AST as a class template declaration
     * 
     * @param class_template_node node to process
     * @param scope current scope object
     * @return ASTInfo info scraped from this node and child nodes
     */
    ASTInfo scrape_class_template_decl_info (json& class_template_node, Scope& scope);

    /**
     * @brief Process this node of the AST as a class template specialization
     * 
     * @param class_template_node node to process
     * @param scope current scope object
     * @return ASTInfo info scraped from this node and child nodes
     */
    ASTInfo scrape_class_template_spec_info (json& class_template_node, Scope& scope);

    /**
     * @brief Process this node of the AST as a field declaration
     * 
     * @param field_node node to process
     * @param scope current scope object
     * @return FieldInfo info scraped from this field
     */
    FieldInfo scrape_field_decl_info (json& field_node, Scope& scope);
}