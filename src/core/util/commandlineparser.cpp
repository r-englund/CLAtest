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
        outputValueArg_ = new TCLAP::ValueArg<std::string>("o",
            "outputPath",
            "Specify output path",
            false,
            "",
            "Output path");
        pythonScriptArg_ = new TCLAP::ValueArg<std::string>("p",
                                                            "pythonScript",
                                                            "Specify a python script to run at startup",
                                                            false,
                                                            "",
                                                            "Path to the file containing the script");
        snapshotArg_ = new TCLAP::ValueArg<std::string>("s",
            "snapshot",
            "Specify default name of each snapshot, or empty string for processor name",
            false,
            "",
            "Snapshot default name: UPN=Use Processor name.");
        quitValueArg_ = new TCLAP::SwitchArg("q", "quit", 
            "Pass this flag if you want to close inviwo after startup.");
        
        cmd_->add(*workspaceValueArg_);
        cmd_->add(*pythonScriptArg_);
        cmd_->add(*outputValueArg_);
        cmd_->add(*snapshotArg_);
        cmd_->add(*quitValueArg_);
    } catch (TCLAP::ArgException &e)  // catch exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}

const std::string CommandLineParser::getOutputPath() const{
    if (outputValueArg_->isSet()){
        return (outputValueArg_->getValue());
    }
    return "";
}

const std::string CommandLineParser::getWorkspacePath() const{
    if (workspaceValueArg_->isSet()){
        return (workspaceValueArg_->getValue());
    }
    return "";
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

bool CommandLineParser::getCaptureAfterStartup() const{
    return snapshotArg_->isSet();
}

const std::string CommandLineParser::getSnapshotName() const{
    if (snapshotArg_->isSet()){
        return (snapshotArg_->getValue());
    }
    return "";
}


bool CommandLineParser::getRunPythonScriptAfterStartup() const{
    return pythonScriptArg_->isSet();
}

const std::string CommandLineParser::getPythonScirptName() const{
    if(pythonScriptArg_->isSet()){
        return (pythonScriptArg_->getValue());
    }
    return "";
        
}

bool CommandLineParser::getQuitApplicationAfterStartup() const{
    return quitValueArg_->getValue();
}

bool CommandLineParser::getLoadWorkspaceFromArg() const{
    if (workspaceValueArg_->isSet()){
        std::string values = workspaceValueArg_->getValue();

        assert(values.size() != 0);
        return true;
    }
    return false;
}

void CommandLineParser::deinitialize() {
}

} // namespace
