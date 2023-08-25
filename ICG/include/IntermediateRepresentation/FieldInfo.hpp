#pragma once

#include <map>
#include <vector>

#include "utils.hpp"
#include "ICGTemplateEngine/ICGTemplateEngine.hpp"

/**
 * @brief Hold information about a field declaration
 * 
 */
class FieldInfo final : public ICGTemplateEngine::recursable {
    public:
    FieldInfo ();

    // Rule of 5 is default

    std::string type;
    std::string name;
    
    ICGTemplateEngine::Dictionary toDictionary() const override;
    ICGTemplateEngine::ListTokenItems nextLevel() const override;

    std::string toString() const override;
};