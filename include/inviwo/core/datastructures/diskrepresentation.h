#ifndef IVW_DISKREPRESENTATION_H
#define IVW_DISKREPRESENTATION_H

#include <inviwo/core/inviwocoredefine.h>
#include <string>

namespace inviwo {

    class IVW_CORE_API DiskRepresentation {

    public:
        DiskRepresentation(){ sourceFile_=""; };
        DiskRepresentation(std::string srcFile){ sourceFile_=srcFile; };
        virtual ~DiskRepresentation(){};
        std::string getSourceFile(){ return sourceFile_; };
        bool hasSourceFile() { return !sourceFile_.empty(); };
	private:
		std::string sourceFile_;
    };

} // namespace

#endif // IVW_DISKREPRESENTATION_H
