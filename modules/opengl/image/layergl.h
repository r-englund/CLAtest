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

#ifndef IVW_LAYERGL_H
#define IVW_LAYERGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/layerrepresentation.h>
#include <modules/opengl/glwrap/texture2d.h>

namespace inviwo {

class Shader;
class IVW_MODULE_OPENGL_API LayerGL : public LayerRepresentation {

public:
    LayerGL(uvec2 dimensions = uvec2(256,256), LayerType type = COLOR_LAYER, const DataFormatBase* format = DataVec4UINT8::get(), Texture2D* tex = NULL);
    LayerGL(const LayerGL& rhs);
    LayerGL& operator=(const LayerGL& rhs);
    virtual ~LayerGL();
    virtual LayerGL* clone() const;
    
    virtual std::string getClassName() const { return "LayerGL"; }

    void bindTexture(GLenum texUnit) const;
    void unbindTexture() const;

    virtual bool copyAndResizeLayer(DataRepresentation*) const;
    virtual void resize(uvec2 dimensions);

    Texture2D* getTexture();
    const Texture2D* getTexture() const;

protected:
    void initialize();
    void deinitialize();

private:
    Texture2D* texture_;
};

} // namespace

#endif // IVW_LAYERGL_H
