#ifndef IVW_VOLUME_H
#define IVW_VOLUME_H

#include "inviwo/core/datastructures/data.h"

namespace inviwo {

    class Volume : public Data {

    public:
        Volume();
        virtual ~Volume();
        Data* clone();        
        void setDimension(ivec3 dim);
        void setFormat(std::string format); 
        ivec3 getDimension();
        std::string getFormat();
    };

} // namespace

#endif // IVW_VOLUME_H
