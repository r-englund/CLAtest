#ifndef IVW_VOLUME_H
#define IVW_VOLUME_H

#include "inviwo/core/datastructures/data.h"

namespace inviwo {

    class Volume : public Data {

    public:
        Volume();
        virtual ~Volume();

    private:
        tgt::ivec3 dimensions_;
    };

} // namespace

#endif // IVW_VOLUME_H
