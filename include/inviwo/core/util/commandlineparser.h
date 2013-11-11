#ifndef IVW_COMMANDLINEPARSER_H
#define IVW_COMMANDLINEPARSER_H

#include <inviwo/core/common/inviwo.h>
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
    CommandLineParser(int argc, char** argv);
    ~CommandLineParser();
    
    void initialize();
    void deinitialize();

    void parse(int argc, char** argv);

    void parse();

    void setArgc(int argc) {
        argc_ = argc;
    }

    void setArgv(char** argv) {
        argv_ = argv;
    }
    const std::string getOutputPath() const;
    const std::string getWorkspacePath() const;
    bool getCaptureAfterStartup() const;
    const std::string getSnapshotName() const;
    bool getRunPythonScriptAfterStartup() const;
    const std::string getPythonScirptName() const;
    bool getQuitApplicationAfterStartup() const;
    bool getLoadWorkspaceFromArg() const;
	bool getShowSplashScreen() const;
    
private:
    int argc_;
    char** argv_;
    TCLAP::CmdLine *cmd_;
    TCLAP::ValueArg<std::string>* workspaceValueArg_;
    TCLAP::ValueArg<std::string>* outputValueArg_;
    TCLAP::ValueArg<std::string>* snapshotArg_;
    TCLAP::ValueArg<std::string>* pythonScriptArg_;
	TCLAP::SwitchArg* noSplashScreenArg_;
    TCLAP::SwitchArg* quitArg_;
    std::string workspaceName_;

};

} // namespace

#endif // IVW_COMMANDLINEPARSER_H
