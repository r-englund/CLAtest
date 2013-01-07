#ifndef IVW_VOLUMEDISK_H
#define IVW_VOLUMEDISK_H

#include "inviwo/core/datastructures/volumerepresentation.h"
#include <stdio.h>
#include <fstream>

namespace inviwo {

    class VolumeDisk : public VolumeRepresentation {

    public:
        VolumeDisk(std::string url);
        virtual ~VolumeDisk();
        virtual void initialize();
        virtual void deinitialize();
        virtual DataRepresentation* clone();
        virtual std::string getUrl() {return url_;}
        virtual std::string getFileName() {return fileName_;}

        template <class T>
        T* loadData() {

            T* texels = new T[dimensions_.x*dimensions_.y*dimensions_.z];

            std::fstream fin(fileName_.c_str(), std::ios::in | std::ios::binary);
            ivwAssert(fin.good(), "cannot open volume file");
            fin.read((char*)texels, dimensions_.x*dimensions_.y*dimensions_.z);
            fin.close();
            return texels;
        }

    private:
        std::string url_;
        std::string fileName_;
    };

} // namespace

#endif // IVW_VOLUMEDISK_H
