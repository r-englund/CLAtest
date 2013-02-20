#ifndef IVW_IMAGEPORT_H
#define IVW_IMAGEPORT_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/ports/dataport.h>
#include <inviwo/core/datastructures/image.h>

namespace inviwo {

class IVW_CORE_API ImagePort : public DataPort<Image> {

public:
    ImagePort(Port::PortDirection direction, std::string identifier);
    ~ImagePort();

    void initialize();
    void deinitialize();

    void resize(uvec2 dimensions);
    uvec2 getDimensions() { return dimensions_; }

    virtual Image* getData();

    virtual uvec3 getColorCode() { return uvec3(90,127,183); }

private:
    uvec2 dimensions_;
    std::map<std::string, Image*> imageDataMap_;

    Image* resizeImageData(std::string processorID, uvec2 targetDim);
};

} // namespace

#endif // IVW_IMAGEPORT_H
