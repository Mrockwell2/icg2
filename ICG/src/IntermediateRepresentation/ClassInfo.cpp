#include "IntermediateRepresentation/ClassInfo.hpp"
#include "utils.hpp"

/************************************/
/*       ClassInfo Definitions      */
/************************************/

ClassInfo::ClassInfo() {}
ClassInfo::ClassInfo(std::string n) : name(n) {}

ICGTemplateEngine::Dictionary ClassInfo::toDictionary() const {
    ICGTemplateEngine::Dictionary dictionary;

    std::stringstream fullname;
    fullname << name;
    if (template_args.size() != 0) {
        fullname << "<";
        for (int i = 0; i < template_args.size(); i++) {
            if (i != 0) {
                fullname << ", ";
            }
            fullname << template_args[i];
        }
        fullname << ">";
    }

    dictionary["ClassName"] = fullname.str();
    dictionary["ClassName_mangled"] = ICGUtils::makeVarname(fullname.str());

    return dictionary;
}

// Push the fields
ICGTemplateEngine::ListTokenItems ClassInfo::nextLevel() const {
    ICGTemplateEngine::ListTokenItems my_fields_dictionary;
    std::string field_key = "fields";

    for (const auto& field : fields) {
        my_fields_dictionary[field_key].push_back(&field);
    }
    return my_fields_dictionary;
}

std::string ClassInfo::toString () const {
    std::stringstream fullname;
    fullname << name;
    if (template_args.size() != 0) {
        fullname << "<";
        for (int i = 0; i < template_args.size(); i++) {
            if (i != 0) {
                fullname << ", ";
            }
            fullname << template_args[i];
        }
        fullname << ">";
    }

    std::string ret = "class " + fullname.str() + " {\n";
    for (auto field : fields) {
        ret += "\t" + field.toString() + "\n";
    }

    ret += "};\n";
    return ret;
}

std::unordered_set<std::string> ClassInfo::getStlMembers() const {

    std::unordered_set<std::string> ret;

    for (auto field : fields) {
        // Did we find an stl type?
        if (ICGUtils::isStlContainer(field.type)) {
            ret.insert(field.type);
        }
    }

    return ret;
}
