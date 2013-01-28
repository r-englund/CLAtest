#ifndef IVW_VOLUME_H
#define IVW_VOLUME_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

    class IVW_CORE_API Volume : public Data3D {

    public:
        Volume();
        virtual ~Volume();
        Data* clone()=0;        
        //void setDimension(ivec3 dim);
        void setFormat(std::string format); 
        //ivec3 getDimension();
        std::string getFormat();
    };

} // namespace

#endif // IVW_VOLUME_H
