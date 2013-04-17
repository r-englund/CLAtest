#ifndef IVW_RAWVOLUMEREADER_H
#define IVW_RAWVOLUMEREADER_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/volumereader.h"

namespace inviwo {

struct ReaderSettings {            
    std::string rawFileAbsolutePath_;
    uvec3 dimensions_;
    std::string dataFormat_;
    ReaderSettings(std::string rawFile, ivec3 resolution=ivec3(0), std::string format="UCHAR");
    ReaderSettings() {}
};

class IVW_CORE_API RawVolumeReader {
public:
    RawVolumeReader();
    virtual ~RawVolumeReader() {}

   static  void* loadRawData(ReaderSettings& readerSettings);

protected:
    template <class T>
    static T* loadData(std::string rawFileAbsolutePath, uvec3 dimensions) {
        T* texels = new T[dimensions.x * dimensions.y * dimensions.z];
        std::fstream fin(rawFileAbsolutePath.c_str(), std::ios::in | std::ios::binary);
        ivwAssert(fin.good(), "cannot open volume file");
        fin.read((char*)texels, dimensions.x*dimensions.y*dimensions.z);
        fin.close();
        return texels;
    }

};

} // namespace

#endif // IVW_RAWVOLUMEREADER_H
