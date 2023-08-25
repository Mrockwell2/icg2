#include "JClang/Traversal.hpp"
#include "JClang/JClang.hpp"

namespace JClang {


    ASTInfo traverseAST (json& ast) {
        Scope scope;
        return scrape_ast(ast, scope);
    }

    ASTInfo scrape_ast (json& ast_node, Scope& scope) {

        ASTInfo total_result;
        for (auto item : ast_node["inner"]) {

            switch (getNodeKind(item)) {
                case ClassTemplateDecl: {
                    total_result.combine(scrape_class_template_decl_info(item, scope));
                }
                break;
                case ClassDecl: {
                    total_result.combine(scrape_class_info(item, scope));
                }
                break;
                case NamespaceDecl: {
                    scope.push_qualifier(item["name"]);
                    total_result.combine(scrape_ast(item, scope));
                    scope.pop_qualifier();
                }
                break;
                default:
                    std::cout << "Encountered unknown kind: " << item["kind"] << std::endl;
            }
        }

        return total_result;
    }

    ASTInfo scrape_class_info (json& class_node, Scope& scope) {
        ASTInfo result;

        ClassInfo * info = new ClassInfo;
        info->name = scope.make_scoped_name(class_node["name"]);

        for (auto item : class_node["inner"]) {
            
            switch (getNodeKind(item)) {
                case FieldDecl: {
                    info->fields.push_back(scrape_field_decl_info(item, scope));
                }
                break;
                case ClassDecl: {
                    scope.push_qualifier(class_node["name"]);
                    result.combine(scrape_class_info(item, scope));
                    scope.pop_qualifier();
                }
                break;
                default: ;
            }
        }

        result.add_class_info(info);

        return result;
    }


    ASTInfo scrape_class_template_decl_info (json& class_template_node, Scope& scope) {
        ASTInfo result;

        std::string template_name = scope.make_scoped_name(class_template_node["name"]);
        int num_template_args = 0;

        for (auto item : class_template_node["inner"]) {

            switch (getNodeKind(item)) {
                case TemplateParameterDecl: {
                    num_template_args++;
                }
                break;
                case ClassTemplateSpecialization: {
                    result.combine(scrape_class_template_spec_info(item, scope));
                }
                break;
                default: ;
                
            }
        }

        return result;                                                                                                                                                                                                                                                                                                                                       
    }

    ASTInfo scrape_class_template_spec_info (json& class_template_node, Scope& scope) {
        ASTInfo result;

        ClassInfo * info = new ClassInfo;
        info->name = class_template_node["name"];

        for (auto template_item : class_template_node["inner"]) {
            switch (getNodeKind(template_item)) {
                case TemplateArgument: {
                    info->template_args.push_back(getQualifiedTypeOfField(template_item));
                }
                break;
                case FieldDecl: {
                    info->fields.push_back(scrape_field_decl_info(template_item, scope));
                }
                break;
                default: ;
            }
        }

        result.add_class_info(info);

        return result;
    }

    FieldInfo scrape_field_decl_info (json& field_node, Scope& scope) {
        FieldInfo field;
        field.name = field_node["name"];
        field.type = getQualifiedTypeOfField(field_node);
        return field;
    }
}