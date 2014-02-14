 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

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
    ImageInport(std::string identifier, bool outportDeterminesSize=false, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~ImageInport();

    void initialize();
    void deinitialize();

    void changeDataDimensions(ResizeEvent* resizeEvent);
    uvec2 getDimension() const;
    const Image* getData() const;
    uvec3 getColorCode() const;
    bool isOutportDeterminingSize() const;
    static uvec3 colorCode;
    virtual std::string getClassName() const {return "ImageInport";}
protected:
    void propagateResizeToPredecessor(ResizeEvent* resizeEvent);

private:
    uvec2 dimensions_;
    bool outportDeterminesSize_;
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
