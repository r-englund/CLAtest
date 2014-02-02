/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_IMAGEPORT_H
#define IVW_IMAGEPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/interaction/events/eventhandler.h>
#include <inviwo/core/interaction/events/resizeevent.h>

namespace inviwo {

class CanvasProcessor;

class IVW_CORE_API ImageInport : public DataInport<Image> {

public:
    ImageInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~ImageInport();

    void initialize();
    void deinitialize();

    void changeDataDimensions(ResizeEvent* resizeEvent);
    uvec2 getDimension() const;
    const Image* getData() const;
    uvec3 getColorCode() const;
    static uvec3 colorCode;
    virtual std::string getClassName() const {return "ImageInport";}
protected:
    void propagateResizeToPredecessor(ResizeEvent* resizeEvent);

private:
    uvec2 dimensions_;
};

class IVW_CORE_API ImageOutport : public DataOutport<Image>, public EventHandler {

    friend class ImageInport;

public:
    ImageOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    ImageOutport(std::string identifier, ImageType type, const DataFormatBase* format = DataVec4UINT8::get(),
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    ImageOutport(std::string identifier, ImageInport* src, ImageType type = COLOR_DEPTH,
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~ImageOutport();

    void initialize();
    void deinitialize();
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);

    Image* getData();

    virtual void dataChanged();

    void changeDataDimensions(ResizeEvent* resizeEvent);
    uvec2 getDimension() const;
    uvec3 getColorCode() const;
    virtual std::string getClassName() const {return "ImageOutport";}

    bool addResizeEventListener(EventListener*);
    bool removeResizeEventListener(EventListener*);

    void setInputSource(LayerType, ImageInport*);

protected:
    Image* getResizedImageData(uvec2 dimensions);
    void setLargestImageData(ResizeEvent* resizeEvent);
    void propagateResizeEventToPredecessor(ResizeEvent* resizeEvent);

    void updateInputSources();

private:
    uvec2 dimensions_;
    bool mapDataInvalid_;
    typedef std::map<std::string, Image*> ImagePortMap;
    ImagePortMap imageDataMap_;
    typedef std::map<LayerType, const ImageInport*> ImageInSourceMap;
    ImageInSourceMap inputSources_;
};

} // namespace

#endif // IVW_IMAGEPORT_H
