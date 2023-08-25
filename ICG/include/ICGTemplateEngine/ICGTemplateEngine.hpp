#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <map>
#include <sstream>

/******************************************/
/*             Data Structures            */
/******************************************/

namespace ICGTemplateEngine {


    /**
     * @brief Map of token name to string it should be replaced with
     * 
     */
    typedef std::map<std::string, std::string> Dictionary;

    class recursable;

    /**
     * @brief Lists that we can recurse into
     * 
     */
    typedef std::map<std::string, std::vector<const recursable *>> ListTokenItems;

    /**
     * @brief An interface for an object that has some attributes that should be used in a list_ token, with the option to nest 
     * 
     */
    class recursable {
        public:

        /**
         * @brief Get a map of token name -> token value for this object
         * 
         * @return Dictionary std::map<std::string, std::string> 
         */
        virtual Dictionary toDictionary() const = 0;

        /**
         * @brief Get the value of any recursable objects nested in this one. May return empty if this is a "leaf"
         * 
         * @return ListTokenItems std::map<std::string, std::vector<const recursable *>>
         */
        virtual ListTokenItems nextLevel() const = 0;

        /**
         * @brief Get a string representation
         * 
         * @return std::string 
         */
        virtual std::string toString() const = 0;
    };

    /**
     * @brief 
     * 
     * @param template_to_fill top level template to fill in
     * @param token_dictionary map of token names to string values they expand to. Values may contain more tokens.
     * @param recursable_list List of special objects that implement recursable that should be used to fill in list_ tokens
     * @return std::string 
     */
    std::string process(const std::string& template_to_fill, const Dictionary& token_dictionary, const ListTokenItems& recursable_map);

    /**
     * @copybrief std::string process(const std::string& template_to_fill, const Dictionary& token_dictionary, const std::vector<const recursable *>& recursable_list);
     * Uses token named "top" in token_dictionary to begin processing.
     * 
     * @param token_dictionary map of token names to string values they expand to. Values may contain more tokens.
     * @param recursable_list List of special objects that implement recursable that should be used to fill in list_ tokens
     * @return std::string 
     */
    std::string process(const Dictionary& token_dictionary, const ListTokenItems& recursable_list);
}


