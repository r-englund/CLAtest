#ifndef IVW_COMMANDLINEPARSER_H
#define IVW_COMMANDLINEPARSER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

#undef HAVE_CONFIG_H
#include <tclap/CmdLine.h>

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
    virtual ~CommandLineParser();

    virtual void initialize();
    virtual void deinitialize();

    void parse(int argc, char** argv) {
        cmd_.parse(argc, argv);
    }

    void setArgc(int argc) {
        argc_ = argc;
    }

    void setArgv(char** argv) {
        argv_ = argv;
    }
    //virtual std::string getWorkspace();
    //virtual bool getExitWithCapture();
    // TODO: No need to serialize the parser, right?
    //virtual void serialize(IvwSerializer& s) const;
    //virtual void deserialize(IvwDeserializer& d);

protected:
    TCLAP::CmdLine cmd_;

private:
    int argc_;
    char** argv_;
};

} // namespace

#endif // IVW_COMMANDLINEPARSER_H
