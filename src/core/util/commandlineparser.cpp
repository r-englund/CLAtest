#include <inviwo/core/util/commandlineparser.h>

namespace inviwo {

    CommandLineParser::CommandLineParser()
        {
    //cmd_("Inviwo description...", ' ', "0.0.0");
    }

CommandLineParser::~CommandLineParser() {}

void CommandLineParser::initialize() {
    // Set up available arguments and flags
    //TCLAP::ValueArg<std::string> workspaceArg("o", "workspace", "Workspace to open", false, "", "Path to workspace");
    //cmd_.add( workspaceArg );
}

void CommandLineParser::deinitialize() {
}

} // namespace
