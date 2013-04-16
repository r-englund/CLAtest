#ifndef IVW_IMAGEPORT_H
#define IVW_IMAGEPORT_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/ports/dataport.h>
#include <inviwo/core/datastructures/image.h>

namespace inviwo {

class IVW_CORE_API ImagePort : public DataPort<Image> {

public:
    ImagePort(Port::PortDirection direction, std::string identifier);
    virtual ~ImagePort();

    void initialize();
    void deinitialize();

    void changeDimensions(uvec2 dimensions);
    uvec2 getDimensions() { return dimensions_; }

    const Image* getData() const;

    uvec3 getColorCode() const { return uvec3(90,127,183); }

private:
    uvec2 dimensions_;
    std::map<std::string, Image*> imageDataMap_;

    Image* resizeImageData(uvec2 dimensions);
};

} // namespace

#endif // IVW_IMAGEPORT_H
