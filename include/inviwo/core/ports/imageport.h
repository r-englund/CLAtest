#ifndef IVW_IMAGEPORT_H
#define IVW_IMAGEPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/datastructures/image.h>

namespace inviwo {

class IVW_CORE_API ImageInport : public DataInport<Image> {

public:
    ImageInport(std::string identifier);
    virtual ~ImageInport();

    void initialize();
    void deinitialize();

    void changeDimensions(uvec2 dimensions);
    uvec2 getDimensions() const;

    const Image* getData() const;

    uvec3 getColorCode() const;

private:
    uvec2 dimensions_;
};

class IVW_CORE_API ImageOutport : public DataOutport<Image> {

friend class ImageInport;

public:
    ImageOutport(std::string identifier);
    virtual ~ImageOutport();

    void initialize();
    void deinitialize();

    void changeDimensions(uvec2 dimensions);
    uvec2 getDimensions() const;

    uvec3 getColorCode() const;

protected:
    Image* resizeImageData(uvec2 dimensions);

private:
    uvec2 dimensions_;
    std::map<std::string, Image*> imageDataMap_;

};

} // namespace

#endif // IVW_IMAGEPORT_H
