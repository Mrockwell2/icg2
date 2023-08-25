
#include <iostream>
#include "JClang/JClang.hpp"

namespace JClang {

    NodeKind getNodeKind (json& node) {
        if (node["kind"] == "CXXRecordDecl" ) {
            if ( node["tagUsed"] == "class") {
                if (node.contains("isImplicit")) {
                    if (node["isImplicit"] == true) {
                        return Unknown;
                    }
                } else {
                    return ClassDecl;
                }
            }
        }

        if (node["kind"] == "FieldDecl") {
            return FieldDecl;
        }

        if (node["kind"] == "TemplateTypeParmDecl") {
            return TemplateParameterDecl;
        }

        if (node["kind"] == "ClassTemplateSpecializationDecl") {
            return ClassTemplateSpecialization;
        }

        if (node["kind"] == "TemplateArgument") {
            return TemplateArgument;
        }

        if (node["kind"] == "ClassTemplateDecl") {
            return ClassTemplateDecl;
        }

        if (node["kind"] == "NamespaceDecl") {
            return NamespaceDecl;
        }

        return Unknown;
    }

    std::string getQualifiedTypeOfField (json& field_node) {

        if (!(getNodeKind(field_node) == FieldDecl || getNodeKind(field_node) == TemplateArgument )) {
            std::cerr << "Attempted to get type of node that is not a field declaration or template argument." << std::endl;
            return "ERROR_TYPE";
        }

        if (field_node["type"].contains("desugaredQualType")) {
            return field_node["type"]["desugaredQualType"];
        }
        return field_node["type"]["qualType"];
    }
}