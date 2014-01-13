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

#ifndef IVW_IMAGEGL_H
#define IVW_IMAGEGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <modules/opengl/image/layergl.h>
#include <modules/opengl/glwrap/framebufferobject.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class Shader;
class IVW_MODULE_OPENGL_API ImageGL : public ImageRepresentation {

public:
    ImageGL();
    ImageGL(const ImageGL& rhs);
    virtual ~ImageGL();

    void initialize();
    void deinitialize();
    virtual DataRepresentation* clone() const;
    virtual std::string getClassName() const { return "ImageGL"; }

    void reAttachAllLayers(bool clearLayers = false);

    void activateBuffer();
    void deactivateBuffer();
   
    virtual bool copyAndResizeRepresentation(DataRepresentation*) const;
    bool updateFrom(const ImageGL*);

    FrameBufferObject* getFBO();
    const FrameBufferObject* getFBO() const;

    LayerGL* getColorLayerGL(size_t idx = 0);
    LayerGL* getDepthLayerGL();
    LayerGL* getPickingLayerGL();

    const LayerGL* getColorLayerGL(size_t idx = 0) const;
    const LayerGL* getDepthLayerGL() const;
    const LayerGL* getPickingLayerGL() const;

protected:
    virtual void update(bool editable);

private:
    std::vector<LayerGL*> colorLayersGL_;
    LayerGL* depthLayerGL_;
    LayerGL* pickingLayerGL_;

    FrameBufferObject* frameBufferObject_;
    GLenum pickingAttachmentID_;
    Shader* program_;
};

} // namespace

#endif // IVW_IMAGEGL_H
