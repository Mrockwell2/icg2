#pragma once

#include <deque>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

class MutableVariableName {
    public:
        MutableVariableName() {}
        MutableVariableName(std::string name) {
            try {
                parse_str(name);
            } catch (std::exception& e) {
                // re-throw with better message
                std::string message = "Parsing " + name + " failed with error: " + e.what();
                throw std::logic_error(message);
            }
        }

        void pushName(std::string name) {
            nameElements.push_back(name);
        }
        void pushIndex(int i) {
            nameElements.push_back(std::string("[" + std::to_string(i) + "]"));
        }

        std::string pop_back() {
            std::string ret = nameElements.back();
            nameElements.pop_back();
            return ret;
        }

        std::string pop_front() {
            std::string ret = nameElements.front();
            nameElements.pop_front();
            return ret;
        }

        std::string front() {
            return nameElements.front();
        }

        int size() {
            return nameElements.size();
        }

        bool empty () {
            return nameElements.empty();
        }

        std::vector<std::string> getElements() const {
            return std::vector<std::string>(nameElements.begin(), nameElements.end());
        }

        std::string toString() const {
            if (nameElements.empty()) {
                return "";
            }

            std::stringstream ss;

            ss << nameElements[0];
            for (unsigned int i = 1; i < nameElements.size(); i++) {
                if (nameElements[i].at(0) != '[') {
                    ss << ".";
                } 

                ss << nameElements[i];
            }

            return ss.str();
        }

    private:
        std::deque<std::string> nameElements;

        // Tail recursion to parse
        void parse_str(std::string name) {

            // Nothing left - we're done!
            if (name.size() == 0) {
                return;
            }

            size_t dot_pos = name.find(".", 0);
            size_t index_pos = name.find("[", 0);

            std::string rest_of_name;

            if (dot_pos == std::string::npos && index_pos == std::string::npos) {
                // There are no more . or [ chars, so everything left is a single name.
                pushName(name);

                // We could return here really, but we want to preserve the tail recursive structure 
                rest_of_name = "";

            } else if (index_pos == 0) {
                // Index operator is next thing
                size_t end_index_pos = name.find("]", 0);
                if (end_index_pos == std::string::npos) {
                    // uh oh
                    throw std::logic_error("Missing end bracket");
                }

                size_t number_length = end_index_pos - (index_pos+1);
                std::string number = name.substr(index_pos+1, number_length);
                long int index;

                try {
                    index = stoi(number);
                } catch (std::exception& e) {
                    // Re-throw with a more relavent error message
                    std::string message = "Index brackets contain \"" + number + "\", which cannot be parsed as an unsigned integer.";
                    throw std::logic_error(message);
                }

                if (index < 0) {
                    std::string message = "Index brackets contain a negative number (" + number + ").";
                    throw std::logic_error(message);
                }

                pushIndex(index);

                rest_of_name = name.substr(end_index_pos+1);
            } else if (dot_pos == 0) {
                // If the pos is 0 just slice if off and don't push anything
                // for example var[0].a
                rest_of_name = name.substr(1);
            } else {
                // Get the name until dot or bracket

                // Woo use a lambda as an assignment
                // We already know that both cannot be npos and neither can be 0
                size_t name_end_index = [=]() -> size_t {
                    if (index_pos == std::string::npos) return dot_pos;
                    if (dot_pos == std::string::npos) return index_pos;
                    return std::min(dot_pos, index_pos);
                }();
                
                std::string subname = name.substr(0, name_end_index);
                pushName(subname);

                rest_of_name = name.substr(name_end_index);
            }

            // Tail recursion!
            parse_str(rest_of_name);
        }
};