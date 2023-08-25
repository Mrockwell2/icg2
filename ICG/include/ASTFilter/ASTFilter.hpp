#include <regex>
#include <iostream>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ASTFilter {

    /**
     * @brief Keep track of what is a system header by pulling system include directories from `clang -### <file` 
     * 
     */
    class SourceManager {
        public:
        /**
         * @brief Construct a new Source Manager object
         * 
         * @param file file that we're trying to manage
         */
        SourceManager(std::string file);

        /**
         * @brief Determine whether a given full file path is a system header or not
         * 
         * @param file full file path, as printed in an AST
         * @return true file is a system header
         * @return false otherwise
         */
        bool isSystem(std::string file);

        /**
         * @brief Print out the system header directories.
         * 
         */
        void print();

        private:

        /**
         * @brief Determine what directories the system headers live in.
         * 
         * @param file file to compile with
         */
        void init (std::string file);

        std::set<std::string> system_includes;
    };

    json generateFullAST (std::string file);
    json generateFilteredAST (std::string file);

    void filter_ast(json& full_ast, SourceManager& source_manager);
}

