#ifndef IVW_IMAGEPORT_H
#define IVW_IMAGEPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/interaction/events/resizeevent.h>

namespace inviwo {

class CanvasProcessor;

class IVW_CORE_API ImageInport : public DataInport<Image> {

public:
    ImageInport(std::string identifier);
    virtual ~ImageInport();

    void initialize();
    void deinitialize();

    void changeDataDimensions(ResizeEvent* resizeEvent);    
    uvec2 getDimensions() const;
    const Image* getData() const;
    uvec3 getColorCode() const;

protected:
    void propagateResizeToPredecessor(ResizeEvent* resizeEvent);

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
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
    
    void changeDataDimensions(ResizeEvent* resizeEvent);    
    uvec2 getDimensions() const;
    uvec3 getColorCode() const;    
    
protected:
    Image* getResizedImageData(uvec2 dimensions);
    void setLargestImageData();
    void propagateResizeEventToPredecessor(ResizeEvent* resizeEvent);

private:
    uvec2 dimensions_;
    bool mapDataInvalid_;
    typedef std::map<std::string, Image*> ImagePortMap;
    ImagePortMap imageDataMap_;
};

} // namespace

#endif // IVW_IMAGEPORT_H
