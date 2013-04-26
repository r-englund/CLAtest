#include <inviwo/core/util/commandlineparser.h>

namespace inviwo {

CommandLineParser::CommandLineParser(){
    cmd_ = new TCLAP::CmdLine("Inviwo description...", ' ', "0.0.0");
}

CommandLineParser::CommandLineParser(int argc, char **argv) : argc_(argc), argv_(argv) 
{
    cmd_ = new TCLAP::CmdLine("Inviwo description...", ' ', "0.0.0");
}

CommandLineParser::~CommandLineParser() {
}

void CommandLineParser::initialize() {
    // Set up available arguments and flags
    try{  
        workspaceValueArg_ = new TCLAP::ValueArg<std::string>("w",
                                                            "workspacePath",
                                                            "Specify workspace to open",
                                                            false,
                                                            "",
                                                            "Name of workspace");
        snapshotCloseArg_ = new TCLAP::SwitchArg("s", "snapshot", 
            "Pass this flag if you want to take snapshots on all canvases and then close inviwo.");
        cmd_->add( *snapshotCloseArg_ );
        cmd_->add( *workspaceValueArg_ );
    } catch (TCLAP::ArgException &e)  // catch exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}
const std::string CommandLineParser::getWorkspace() const{
    if(workspaceValueArg_->isSet()){
        return (workspaceValueArg_->getValue());
    }
    return NULL;
}

void CommandLineParser::parse(int argc, char** argv){
    try{        
        cmd_->parse(argc, argv);
    } catch (TCLAP::ArgException &e) { 
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; // catch exceptions 
    }
}

void CommandLineParser::parse(){
    parse(argc_, argv_);
}

bool CommandLineParser::getExitWithCapture() const{
    return snapshotCloseArg_->getValue();
}

bool CommandLineParser::getLoadWorkspaceFromArg() const{
    if(workspaceValueArg_->isSet()){
        std::string values = workspaceValueArg_->getValue();

        assert(values.size() != 0);
        return true;
    }
    return false;
}

void CommandLineParser::deinitialize() {
}

} // namespace
