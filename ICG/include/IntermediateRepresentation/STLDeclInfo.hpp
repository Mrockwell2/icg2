#pragma once

#include <map>
#include <vector>

#include "utils.hpp"
#include "ICGTemplateEngine/ICGTemplateEngine.hpp"

/**
 * @brief Hold information about a field declaration
 * 
 */
class STLDeclInfo final : public ICGTemplateEngine::recursable {
    public:
    STLDeclInfo ();
    STLDeclInfo (std::string n);

    // Rule of 5 is default
    std::string name;
    
    ICGTemplateEngine::Dictionary toDictionary() const override;
    ICGTemplateEngine::ListTokenItems nextLevel() const override;
    std::string toString() const override;
};