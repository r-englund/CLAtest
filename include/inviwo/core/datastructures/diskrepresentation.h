#ifndef IVW_DISKREPRESENTATION_H
#define IVW_DISKREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <string>
#include <inviwo/core/io/datareader.h>

namespace inviwo {

    class IVW_CORE_API DiskRepresentation {

    public:
        DiskRepresentation() : sourceFile_(""), reader_(NULL) {};
        DiskRepresentation(std::string srcFile) : sourceFile_(srcFile), reader_(NULL) {};
        virtual ~DiskRepresentation(){
            if(reader_){
                delete reader_;
            }
        };
        const std::string& getSourceFile() const { return sourceFile_; };
        bool hasSourceFile() const { return !sourceFile_.empty(); };

        void setDataReader(DataReader* reader){
            if(reader_){
                delete reader_;
            }
            reader_ = reader;
        }
        DataReader* getDataReader() const{
            return reader_;
        };
	private:
		std::string sourceFile_;      
        // DiskRepresentation owns a DataReader to be able to convert it self into RAM.
        DataReader* reader_;
    };

} // namespace

#endif // IVW_DISKREPRESENTATION_H
