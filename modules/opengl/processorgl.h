#ifndef IVW_PROCESSORGL_H
#define IVW_PROCESSORGL_H

#include "modules/opengl/openglmoduledefine.h"
#include "inviwoopengl.h"
#include "imagegl.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API ProcessorGL : public Processor {

public:
    ProcessorGL();
    virtual ~ProcessorGL();
    virtual Processor* create() const;

    void activateTarget(ImagePort outport);
    void deactivateCurrentTarget();

    void bindColorTexture(ImagePort inport, GLenum texUnit);
    void unbindColorTexture(ImagePort inport);
    void bindDepthTexture(ImagePort inport, GLenum texUnit);
    void unbindDepthTexture(ImagePort inport);

    void renderImagePlaneQuad() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_PROCESSORGL_H
