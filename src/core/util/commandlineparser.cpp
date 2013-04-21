#include <inviwo/core/util/commandlineparser.h>

namespace inviwo {

CommandLineParser::CommandLineParser(){
    cmd_ = new TCLAP::CmdLine("Inviwo description...", ' ', "0.0.0");
}

CommandLineParser::~CommandLineParser() {
}

void CommandLineParser::initialize() {
    // Set up available arguments and flags
    try{  
        snapshotValueArg_ = new TCLAP::MultiArg<std::string>("w",
                                                            "workspaceSnapshot",
                                                            "Take snapshot on specified workspace",
                                                            false,
                                                            "Name of workspace");
        cmd_->add( *snapshotValueArg_ );
    } catch (TCLAP::ArgException &e)  // catch exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}
std::string CommandLineParser::getWorkspace() const{
    if(snapshotValueArg_->isSet()){
        std::vector<std::string> values = snapshotValueArg_->getValue();
        return *(values.begin());
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

bool CommandLineParser::getExitWithCapture() const{
    if(snapshotValueArg_->isSet()){
        std::vector<std::string> values = snapshotValueArg_->getValue();
        //Should it be possible to print several workspaces at the same time?
        assert(values.size() == 1);

        return true;
        
        /*for(std::vector<std::string>::iterator it = values.begin(); it < values.end(); ++it){
            std::cout << "Got yah!" << (*it) << std::endl;
        }*/
    }
    return false;
}

void CommandLineParser::deinitialize() {
}

} // namespace
