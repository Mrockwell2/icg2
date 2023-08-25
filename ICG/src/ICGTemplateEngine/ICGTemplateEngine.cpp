#include "ICGTemplateEngine/ICGTemplateEngine.hpp"
#include <iostream>

namespace ICGTemplateEngine {

    /******************************************/
    /*       String manipulation helpers      */
    /******************************************/

    // split by given delimeter
    static inline std::vector<std::string> split (const std::string& str, const char delim) {
        std::stringstream ss(str);
        std::string s;
        std::vector<std::string> ret;
        while (std::getline(ss, s, delim)) {
            ret.push_back(s);
        }
        return ret;
    }

    /******************************************/
    /*       Token replacement utilities      */
    /******************************************/

    int get_next_token (const std::string& format, std::string& token) {
        int start_pos = format.find(std::string("{{"), 0);
        if (start_pos == std::string::npos) return -1;
        int end_pos = format.find("}}", 0);

        if (end_pos == std::string::npos || end_pos < start_pos) { throw std::runtime_error("Found token missing an end bracket: " + format); }

        token = format.substr(start_pos+2, end_pos-start_pos-2);

        return start_pos;
    }

    // Shamefully stolen from https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }

    std::string replace_token(const std::string& full, const std::string& token_name, const std::string& replace_val) {
        std::string full_token = "{{" + token_name + "}}";
        int pos = full.find(full_token);

        if (pos == std::string::npos) { throw std::runtime_error("Could not find token: " + full_token); }

        // Try to fix indention
        std::string extra_whitespace = "";
        int whitespace_index = pos-1;
        while (whitespace_index >= 0 && (full.at(whitespace_index) == ' ' || full.at(whitespace_index) == '\t')) whitespace_index--;
        extra_whitespace = "\n" + full.substr(whitespace_index+1, pos-whitespace_index-1);
        std::string indented_replace_val = ReplaceAll(replace_val, "\n", extra_whitespace);


        std::string result = full;
        result.replace(pos, full_token.size(), indented_replace_val);

        return result;
    }


    /*************************************************************/
    /*             Entry points for ICGTemplateEngine            */
    /*************************************************************/

    std::string process(const Dictionary& token_dictionary, const ListTokenItems& recursable_list) {
        auto dictionary_it = token_dictionary.find("top");
        if (dictionary_it == token_dictionary.end()) {
            throw std::logic_error("token_dictionary must have an entry named \"top\" to begin processing.");
        }

        const std::string top_level_token = dictionary_it->second;

        return process(top_level_token, token_dictionary, recursable_list);
    }

    std::string process(const std::string& fmt_string, const Dictionary& token_dictionary, const ListTokenItems& recursable_lists_dictionary) {
        std::string result_string = fmt_string;

        std::string token;
        while (get_next_token(result_string, token) != -1) {
            // First, look up the token in the dictionary
            // If it starts with list, strip it off
            bool list_token = false;
            std::string lookup_token = token;
            std::string list_token_name = "";

            if (token.rfind("list_", 0) == 0) {
                // A list token has the structure of list_recursableName_tokenName

                list_token = true;

                // Chop off the "list_" string
                std::string temp_token = token.substr(5, token.size()-5);

                // Pull out the name of the list to work with
                int loc = temp_token.find("_");
                list_token_name = temp_token.substr(0, loc);
                lookup_token = temp_token.substr(loc+1, temp_token.size()-(loc+1));
            }

            auto token_lookup_val = token_dictionary.find(lookup_token);
            if (token_lookup_val == token_dictionary.end()) { 
                throw std::runtime_error("Got invalid token " + lookup_token + "\nResult string so far: " + result_string); 
            }

            std::string token_val = token_lookup_val->second;

            // Recurse into the token to populate it
            if (list_token) {
                // We'll have to go through the list of functions and concat the results together
                std::stringstream list_result;

                // Find the right list to use
                auto lookup_result = recursable_lists_dictionary.find(list_token_name);
                if (lookup_result == recursable_lists_dictionary.end()) {
                    throw std::runtime_error("No list named " + list_token_name + ".\nResult string so far: " + result_string); 
                }

                auto recursable_list = lookup_result->second;

                for (const recursable * next_level_obj : recursable_list) {
                    Dictionary next_dictionary = token_dictionary;
                    Dictionary obj_dictionary = next_level_obj->toDictionary();
                
                    next_dictionary.insert(obj_dictionary.begin(), obj_dictionary.end());
                    ListTokenItems next_recursable = next_level_obj->nextLevel();
                    
                    list_result << process(token_val, next_dictionary, next_recursable);
                }

                token_val = list_result.str();

            } else {
                token_val = process(token_val, token_dictionary, recursable_lists_dictionary);
            }

            result_string = replace_token(result_string, token, token_val);
        }

        return result_string;
    }
}