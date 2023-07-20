#pragma once

#include <clang-c/Index.h>  // This is libclang.
#include <iostream>
#include <string>

enum StorageClass {LOCAL, STATIC};
enum AccessLevel  {INVALID, PUBLIC, PROTECTED, PRIVATE, NONE};

namespace ICGUtils {

    /**
     * @brief Take a given typename and modify it so that it can be used in a variable name.
     * @note Types are often used as variable names in the code generated by ICG, so we need to
     *          create a version that follow variable name character rules and also won't collide with anything.
     *          Since these are in generated code, they don't have to be pretty, but they should be readable
     *          As a kindness to myself and future Trick developers
     * 
     * @param typename_to_modify 
     * @return std::string version of the type that can be used in a varname
     */
    std::string makeVarname(const std::string& typename_to_modify);

    std::string toStdString(const CXString& str);

    std::ostream& operator<<(std::ostream& stream, const CXString& str);

    std::string getCursorSpelling (CXCursor c);

    std::string getTypeSpelling (CXCursor c);

    std::string getKindSpelling (CXCursor c);

    enum AccessLevel getAccessLevel (CXCursor c);

    std::string makeIOHeaderName (std::string header_name);
}