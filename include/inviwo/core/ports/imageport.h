#ifndef IVW_IMAGEPORT_H
#define IVW_IMAGEPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/datastructures/image.h>
#include <inviwo/core/interaction/events/resizeevent.h>

namespace inviwo {

class CanvasProcessor;

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
    void changeDataDimensions(ResizeEvent* resizeEvent);
    uvec2 getDimensions() const;

    void getDescendantProcessors(std::vector<Processor*>& decendantProcessors);

    uvec3 getColorCode() const;

protected:
    Image* resizeImageData(uvec2 dimensions);
    void setLargestImageData();

private:
    uvec2 dimensions_;
    typedef std::map<std::string, Image*> ImagePortMap;
    ImagePortMap imageDataMap_;


};

} // namespace

#endif // IVW_IMAGEPORT_H
