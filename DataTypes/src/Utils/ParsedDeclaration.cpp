#include "Utils/ParsedDeclaration.hpp"

/*
 type-name: type-specifier
          | type-specifier declarator
          ;

 declarator : pointer
            | pointer direct-declarator
            | direct-declarator
            ;

 direct-declarator: identifier
                  | ( declarator )
                  | [ constant-exp ]
                  | [ ]
                  | direct-declarator [ constant-exp ]
                  | direct-declarator [ ]
                  ;
*/

ParsedDeclaration::ParsedDeclaration(std::string declaration_str) : typeSpec("") {
    LexicalAnalyzer lexer;

    lexer.load(declaration_str);
    parseDeclaration(lexer);
}


// ===============================================
// type-name: type-specifier
//          | type-specifier declarator
//          ;
// ===============================================
bool ParsedDeclaration::parseDeclaration(LexicalAnalyzer& lexer) {

    // std::cout << __FUNCTION__ << std::endl;
    bool errorCondition = parseTypeSpecifier(lexer);
    if ( !errorCondition ) {
        Token::e token = lexer.getToken();
        if ((token == Token::Asterisk) ||
            (token == Token::Identifier ) ||
            (token == Token::LeftParen ) ||
            (token == Token::LeftBracket)) {
            errorCondition = parseDeclarator(lexer);
        }
        if (lexer.matchToken(Token::EndOfFile) == Token::Error) {
            errorCondition = true;
        }
    }
    return errorCondition;
}

// ==============================================================
// declarator : pointer
//            | pointer direct-declarator
//            | direct-declarator
//            ;
//
// NOTE: a pointer is one or more asterisks.
// ==============================================================
bool ParsedDeclaration::parseDeclarator(LexicalAnalyzer& lexer) {
   
//    std::cout << __FUNCTION__ << std::endl;
   bool errorCondition = false;
   int asteriskCount = 0;

   Token::e token = lexer.getToken();
   while (token == Token::Asterisk ) {
       token = lexer.matchToken( Token::Asterisk );
       asteriskCount ++;
   }
   if ((token == Token::LeftParen) || (token == Token::LeftBracket) || (token == Token::Identifier)) {
       errorCondition |= parseDirectDeclarator(lexer);
   }

   if (asteriskCount != 0) {
       for (int ii=0 ; ii < asteriskCount ; ii++) {
           dims.push_back(-1);
       }
   }
   return errorCondition;
}

// ==============================================================
// direct-declarator: identifier
//                  | ( declarator )
//                  | [ integer ]
//                  | direct-declarator [ integer ]
//                  ;
// ==============================================================
bool ParsedDeclaration::parseDirectDeclarator(LexicalAnalyzer& lexer) {
   bool errorCondition = false;

   Token::e token = lexer.getToken();
   if (token == Token::Identifier) {
       varName = lexer.getText();
       token = lexer.matchToken( Token::Identifier );
   } else if (token == Token::LeftParen) {
       token = lexer.matchToken( Token::LeftParen );
       errorCondition |= parseDeclarator(lexer);
       if ( !errorCondition ) {
           if ((token = lexer.matchToken( Token::RightParen )) == Token::Error) {
           errorCondition = true;
           }
       }
   } else if (token == Token::LeftBracket) {
       token = lexer.matchToken( Token::LeftBracket );
       if (token == Token::IntegerLiteral) {
           dims.push_back( std::atoi( (lexer.getText()).c_str() ));
           token = lexer.matchToken(Token::IntegerLiteral);
       }
       if ((token = lexer.matchToken( Token::RightBracket )) == Token::Error) {
           errorCondition = true;
       }
   } else {
       // Note: this is the case of an anonymous declaration.
       varName.clear();
   }

   while (token == Token::LeftBracket) {
       token = lexer.matchToken( Token::LeftBracket );
       if (token == Token::IntegerLiteral) {
           dims.push_back(std::atoi( (lexer.getText()).c_str() ));
           token = lexer.matchToken(Token::IntegerLiteral);
       }
       if ((token = lexer.matchToken( Token::RightBracket )) == Token::Error) {
           errorCondition = true;
       }
   }
   return errorCondition;
}


// ==============================================================
// type-specifier   : sign-qualifier integral-type-keyword
//                  | float-type-keyword
//                  | qualified-identifier
//                  ;
// ==============================================================

// ==============================================================
// sign-qualifier   : <NOTHING>
//                  | signed
//                  | unsigned
// ==============================================================
// am I actually allowed to have <NOTHING> in a bnf grammar?
// probably not

bool ParsedDeclaration::parseTypeSpecifier(LexicalAnalyzer& lexer) {

    // std::cout << __FUNCTION__ << std::endl;
    bool errorCondition = false;
    Token::e token = lexer.getToken();

    switch ( token ) {
        case Token::Void : {
            token = lexer.matchToken(Token::Void);
            setName("void");
        } break;
        case Token::Boolean : {
            token = lexer.matchToken(Token::Boolean);
            setName("bool");
        } break;
        case Token::Char : {
            token = lexer.matchToken(Token::Char);
            setName("char");
        } break;
        case Token::Short : {
            token = lexer.matchToken(Token::Short);
            setName("short");
        } break;
        case Token::Int : {
            token = lexer.matchToken(Token::Int);
            setName("int");
        } break;
        case Token::Long : {
            token = lexer.matchToken(Token::Long);
            if (token == Token::Long) {
                token = lexer.matchToken(Token::Long);
                setName("long long");
            } else {
                setName("long");
            }
        } break;
        case Token::Unsigned : {
            token = lexer.matchToken(Token::Unsigned);
            switch ( token ) {
                case Token::Char : {
                    token = lexer.matchToken(Token::Char);
                    setName("unsigned char");
                } break;
                case Token::Short : {
                    token = lexer.matchToken(Token::Short);
                    setName("unsigned short");
                } break;
                case Token::Int : {
                    token = lexer.matchToken(Token::Int);
                    setName("unsigned int");
                } break;
                case Token::Long : {
                    token = lexer.matchToken(Token::Long);
                    if (token == Token::Long) {
                        token = lexer.matchToken(Token::Long);
                        setName("unsigned long long");
                    } else {
                        setName("unsigned long");
                    }
                } break;
                default: {
                    setName("unsigned int");
                }
            }
        } break;
        case Token::Signed : {
            token = lexer.matchToken(Token::Signed);
            switch ( token ) {
                case Token::Char : {
                    token = lexer.matchToken(Token::Char);
                    setName("char");
                } break;
                case Token::Short : {
                    token = lexer.matchToken(Token::Short);
                    setName("short");
                } break;
                case Token::Int : {
                    token = lexer.matchToken(Token::Int);
                    setName("int");
                } break;
                case Token::Long : {
                    token = lexer.matchToken(Token::Long);
                    if (token == Token::Long) {
                        token = lexer.matchToken(Token::Long);
                        setName("long long");
                    } else {
                        setName("long");
                    }
                } break;
                default: {
                    setName("int");
                }
            }
        } break;
        case Token::WideChar : {
            token = lexer.matchToken(Token::WideChar);
            setName("wchar_t");
        } break;
        case Token::Float : {
            token = lexer.matchToken(Token::Float);
            setName("float");
        } break;
        case Token::Double : {
            token = lexer.matchToken(Token::Double);
            setName("double");
        } break;
        case Token::Identifier : {
            errorCondition |= parseQualifiedIdentifier(lexer);
        } break;
        default : {
            errorCondition = true;
        }
    }
    return errorCondition;
}


// ==============================================================
// qualified-identifier   : identifier
//                        | identifier::qualified-identifier
// ==============================================================
bool ParsedDeclaration::parseQualifiedIdentifier(LexicalAnalyzer& lexer) {

    // We should see an identifier first
    Token::e token = lexer.getToken();
    if (token != Token::Identifier) {
        // This is an error condition
        return true;
    }

    // Parse the identifier
    if (parseIdentifier(lexer)) {
        // Error condition
        return true;
    }

    // Next two tokens must be ::
    // Or something completely different
    token = lexer.getToken();
    if (token != Token::Colon) {
        // Reached the end of the identifier, move on
        return false;
    }

    // Otherwise, we have a colon.
    typeSpec += lexer.getText();
    token = lexer.matchToken(Token::Colon);

    // If this one isn't also a colon, something went bad    
    if (token != Token::Colon) {
        // Error condition has occcurred
        return true;
    }

    // Advance the lexer and do it all over again
    typeSpec += lexer.getText();
    lexer.matchToken(Token::Colon);

    return parseQualifiedIdentifier(lexer);
}


// class-name is a leaf
// template-name is a leaf
// template-argument-list contains Declarations that we must parse (they arent full declarations, just a typespec/anon, but we can still use the same code.)

// ==============================================================
// identifier   : class-name
//              | template-name < template-argument-list >
// ==============================================================
bool ParsedDeclaration::parseIdentifier (LexicalAnalyzer& lexer) {
    // Just throw everything into the identifier for now

    std::string identifier = lexer.getText();

    Token::e token = lexer.matchToken(Token::Identifier);
    if (token == Token::LeftAngle) {
        // Just keep pulling everything into the name until we find the right closing bracket
        identifier += lexer.getText();
        std::string current_param = "";

        int angle_bracket_depth = 1;
        while (angle_bracket_depth != 0) {
            token = lexer.matchToken(token);

            if (token == Token::Comma && angle_bracket_depth == 1) {
                templateParams.emplace_back(current_param);
                current_param = "";
            } else if (token == Token::LeftAngle) {
                angle_bracket_depth++;
                current_param += lexer.getText();
            } else if (token == Token::RightAngle) {
                angle_bracket_depth--;
                if (angle_bracket_depth != 0) {
                    current_param += lexer.getText();
                }
            } else {
                current_param += lexer.getText();
            }

            identifier += lexer.getText();
        }

        templateParams.emplace_back(current_param);
        token = lexer.matchToken(token);
    }
    
    qualifiedTypeNameParts.push_back(identifier);
    typeSpec += identifier;

    return false;
}

std::string ParsedDeclaration::getTypeSpecifier() const {
    return typeSpec;
}

std::deque<std::string> ParsedDeclaration::getQualifiedNameParts() const {
    return qualifiedTypeNameParts;
}


std::string ParsedDeclaration::getVariableName() const {
    return varName;
}

std::vector<int> ParsedDeclaration::getDims() const {
    return dims;
}

std::vector<ParsedDeclaration> ParsedDeclaration::getTemplateParams() const {
    return templateParams;
}

void ParsedDeclaration::setName(std::string name) {
    typeSpec = name;
    qualifiedTypeNameParts = { name };
}