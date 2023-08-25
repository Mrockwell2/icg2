#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <nlohmann/json.hpp>

#include "ICGTemplateEngine/ICGTemplateEngine.hpp"
#include "IntermediateRepresentation/ASTInfo.hpp"
#include "JClang/Traversal.hpp"

#include "ASTFilter/ASTFilter.hpp"

#include "ICG_io_src_templates.hpp"

using json = nlohmann::json;


/****************************************/
/*                Main                  */
/****************************************/

int main(int argc, char ** argv) {

    if (argc != 2) {
        std::cerr << "Usage: icg <headerfile>" << std::endl;
        exit(-1);
    }

    std::string filename = argv[1];

    // Generate the AST
    json ast = ASTFilter::generateFilteredAST(filename);

    // Traverse and pull information out of the AST
    ASTInfo ast_info = JClang::traverseAST(ast);
    ICGTemplateEngine::ListTokenItems top_level_objects_list = ast_info.getItems();

    // Write the io_<headerfile> file
    std::ofstream outfile;
    outfile.open(ICGUtils::makeIOHeaderName(filename));

    // The templates for what the io file should look like live in add-type-templates.hpp
    template_dictionary["filename"] = filename;

    // Runs the template engine to construct the io file using the templates and data gathered from the AST
    try {
        outfile << ICGTemplateEngine::process(template_dictionary, top_level_objects_list);
    } catch (std::exception& ex) {
        std::cerr << "IO file format failed: " << ex.what() << std::endl;
        outfile.close();
        exit(-1);
    }

    outfile.close();
    exit(0);
}