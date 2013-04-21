#ifndef IVW_COMMANDLINEPARSER_H
#define IVW_COMMANDLINEPARSER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

#undef HAVE_CONFIG_H
#include <tclap/CmdLine.h>
#include <string>
#include <vector>

namespace inviwo {

/** \brief Wrapper class to handle command line parsing.
 *  
 *  Wraps around TCLAP to provide command line argument parsing.
 *
 *  TODO: Change into an abstract, general, class and then a derived subclass,
 *  which contains the TCLAP specific parts.
 *  
 *  TODO2: Not yet fully implemented
 */
class IVW_CORE_API CommandLineParser {
    
public:
    CommandLineParser();
    ~CommandLineParser();
    
    void initialize();
    void deinitialize();

    void parse(int argc, char** argv);

    void setArgc(int argc) {
        argc_ = argc;
    }

    void setArgv(char** argv) {
        argv_ = argv;
    }
    std::string getWorkspace() const;
    bool getExitWithCapture()const;

    // TODO: No need to serialize the parser, right?
    //virtual void serialize(IvwSerializer& s) const;
    //virtual void deserialize(IvwDeserializer& d);

    
private:
    int argc_;
    char** argv_;
    TCLAP::CmdLine *cmd_;
    TCLAP::MultiArg<std::string> *snapshotValueArg_;
    std::string workspaceName_;

};

} // namespace

//Temp code, need some rework and a good place to put it.
    /* inviwo::CommandLineParser parser;

    parser.initialize();
    parser.parse(argc, argv);

   if(parser.getExitWithCapture()){
        std::string workspacePath = parser.getWorkspace();
        std::cout << "Capturing workspaces!" << std::endl << "Desired workspace: " << workspacePath << std::endl;
        mainWin.openLastNetwork();
        std::vector<inviwo::CanvasProcessorGL*> pv = inviwoApp.getProcessorNetwork()->getProcessorsByType<inviwo::CanvasProcessorGL>();
        std::cout << "Number of canvases: " << pv.size() << std::endl;

        int i = 0;
        for(std::vector<inviwo::CanvasProcessorGL*>::iterator it = pv.begin(); it != pv.end(); it++){
            std::stringstream ss;
            ss << i;
            std::string path(IVW_DIR+"data/images/" + ss.str() + ".png");
            std::cout << "Saving canvas to: " << path << std::endl;
            (*it)->takeSnapshot((path).c_str());
            ++i;
        }
        
        
        exit(0);    
    }
    else{
        
    }*/

#endif // IVW_COMMANDLINEPARSER_H
