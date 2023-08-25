#pragma once

#include <string>
#include <iostream>
#include <deque>
#include <vector>


#include "Utils/LexicalAnalyzer.hpp"



class ParsedDeclaration {

    public:

    ParsedDeclaration( std::string s );

    std::string getTypeSpecifier() const;
    std::deque<std::string> getQualifiedNameParts() const;
    std::string getVariableName() const;
    std::vector<int> getDims() const;
    std::vector<ParsedDeclaration> getTemplateParams() const;

    private:
    
    bool parseTypeSpecifier(LexicalAnalyzer& lexer);
    bool parseDeclaration(LexicalAnalyzer& lexer);
    bool parseDeclarator(LexicalAnalyzer& lexer);
    bool parseDirectDeclarator(LexicalAnalyzer& lexer);
    bool parseQualifiedIdentifier(LexicalAnalyzer& lexer);
    bool parseIdentifier(LexicalAnalyzer& lexer);
    
    bool match(Token::e expected_token);

    void setName(std::string name);

    std::deque<std::string> qualifiedTypeNameParts;
    std::vector<ParsedDeclaration> templateParams;
    std::string typeSpec;
    std::string varName;
    std::vector<int> dims;
};

