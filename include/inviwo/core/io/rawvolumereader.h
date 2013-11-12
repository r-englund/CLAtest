#ifndef IVW_RAWVOLUMEREADER_H
#define IVW_RAWVOLUMEREADER_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/volumereader.h"

namespace inviwo {

class IVW_CORE_API RawVolumeReader : public VolumeReader {
public:
	RawVolumeReader();
	virtual ~RawVolumeReader() {}

	static void* loadRawData(ReaderSettings& readerSettings);

protected:
    template <class T>

	static T* loadData(std::string rawFileAbsolutePath, uvec3 dimensions) {
        T* texels = new T[dimensions.x * dimensions.y * dimensions.z];
        std::fstream fin(rawFileAbsolutePath.c_str(), std::ios::in | std::ios::binary);
        ivwAssert(fin.good(), "cannot open volume file");
        fin.read((char*)texels, dimensions.x*dimensions.y*dimensions.z*sizeof(T));
        fin.close();
        return texels;
    }
};

} // namespace

#endif // IVW_RAWVOLUMEREADER_H
