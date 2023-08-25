#pragma once

#include <map>
#include <vector>
#include <unordered_set>

#include "ICGTemplateEngine/ICGTemplateEngine.hpp"
#include "IntermediateRepresentation/ClassInfo.hpp"

// Just bundle together all the other things

class ASTInfo {
    public:
    // default constructors

    /**
     * @brief Pull info from another ASTInfo object into self.
     * 
     * @param other another ASTInfo object to absorb
     */
    void combine (const ASTInfo& other);

    /**
     * @brief Add a ClassInfo object
     * 
     * @param class_info fully built ClassInfo object to hold
     */
    void add_class_info(ClassInfo * class_info);

    /**
     * @brief Add an STL declaration
     * 
     * @param stl_name string name of an STL to add (something along the lines of std::vector<int> )
     */
    void add_stl_decl(std::string stl_name);

    /**
     * @brief Add a set of STL declarations
     * 
     * @param stl_names set of string STL names
     */
    void add_stl_decl(std::unordered_set<std::string> stl_names);

    /**
     * @brief Get the representation of this AST that is compatable with the ICGTemplateEngine
     * 
     * @return ICGTemplateEngine::ListTokenItems 
     */
    ICGTemplateEngine::ListTokenItems getItems();

    /**
     * @brief toString
     * 
     * @return std::string 
     */
    std::string toString() const;

    private:
    ICGTemplateEngine::ListTokenItems items;
    std::unordered_set<std::string> stl_decls;

    static const std::string classes_key;
    static const std::string stl_key;
};