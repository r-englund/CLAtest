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

protected:

};

} // namespace

#endif // IVW_RAWVOLUMEREADER_H
