#pragma once

#include <map>
#include <vector>
#include <unordered_set>

#include "utils.hpp"
#include "ICGTemplateEngine/ICGTemplateEngine.hpp"
#include "IntermediateRepresentation/FieldInfo.hpp"

/**
 * @brief Hold information about a class declaration
 * 
 */
class ClassInfo final : public ICGTemplateEngine::recursable {
public:
    ClassInfo();
    ClassInfo(std::string n);

    // Rule of 5 is default

    std::string name;
    std::vector<FieldInfo> fields;
    std::vector<std::string> template_args;

    ICGTemplateEngine::Dictionary toDictionary() const override;
    ICGTemplateEngine::ListTokenItems nextLevel() const override;

    std::unordered_set<std::string> getStlMembers() const;

    std::string toString() const override;
};
