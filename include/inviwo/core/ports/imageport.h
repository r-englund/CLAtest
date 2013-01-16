#ifndef IVW_IMAGEPORT_H
#define IVW_IMAGEPORT_H

#include "inviwo/core/ports/dataport.h"
#include "inviwo/core/datastructures/image.h"

namespace inviwo {

class ImagePort : public DataPort<Image> {

public:
    ImagePort(Port::PortDirection direction, std::string identifier);
    ~ImagePort();

    void initialize();
    void deinitialize();

    void resize(ivec2 dimensions);
    ivec2 getDimensions() { return dimensions_; }

    virtual Image* getData();

    virtual ivec3 getColorCode() { return ivec3(90,127,183); }

private:
    ivec2 dimensions_;
    std::map<std::string, Image*> imageDataMap_;

    Image* resizeImageData(std::string processorID, ivec2 targetDim);
};

} // namespace

#endif // IVW_IMAGEPORT_H
