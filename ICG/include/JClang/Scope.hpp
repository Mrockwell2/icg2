#pragma once

#include <deque>
#include <string>
#include <sstream>

namespace JClang {

    /**
     * @brief Keep track of the namespace scope that we're in
     * 
     */
    class Scope {

        public:
        
        /**
         * @brief Push on a qualifier when we enter a namespace/class
         * 
         * @param name name to push
         */
        void push_qualifier(std::string name) {
            qualified_name_parts.push_back(name);
        }

        /**
         * @brief Pop the top level qualifier when we leave a namespace/class
         * 
         * @return std::string 
         */
        std::string pop_qualifier() {
            std::string result = qualified_name_parts.back();
            qualified_name_parts.pop_back();
            return result;
        }

        /**
         * @brief Take a top level name and attach the current qualifiers
         * 
         * @param name bare name
         * @return std::string fully qualified name
         */
        std::string make_scoped_name(std::string name) {
            std::stringstream ss;
            for (auto part : qualified_name_parts) {
                ss << part << "::";
            }
            ss << name;
            return ss.str();
        }

        private:
        std::deque<std::string> qualified_name_parts;
    };
}