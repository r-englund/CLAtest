#ifndef IVW_VOLUMEDISK_H
#define IVW_VOLUMEDISK_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/volumerepresentation.h>
#include <stdio.h>
#include <fstream>

namespace inviwo {

    class IVW_CORE_API VolumeDisk : public VolumeRepresentation {

    public:
        VolumeDisk(std::string url);
        virtual ~VolumeDisk();
        virtual void initialize();
        virtual void deinitialize();
        virtual DataRepresentation* clone();
        virtual std::string getSourceFile() {return sourceFile_;}
        virtual std::string getRawFile() {return rawFile_;}

        template <class T>
        T* loadData() {

            T* texels = new T[dimensions_.x*dimensions_.y*dimensions_.z];

            std::fstream fin(rawFile_.c_str(), std::ios::in | std::ios::binary);
            ivwAssert(fin.good(), "cannot open volume file");
            fin.read((char*)texels, dimensions_.x*dimensions_.y*dimensions_.z);
            fin.close();
            return texels;
        }

    private:
        std::string sourceFileLocation_;
        std::string sourceFile_;
        std::string rawFile_;
        std::string sourceFileExtension_;
        
    };

} // namespace

#endif // IVW_VOLUMEDISK_H
