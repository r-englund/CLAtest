#ifndef IVW_IMAGEGL_H
#define IVW_IMAGEGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <modules/opengl/glwrap/framebufferobject.h>
#include <modules/opengl/glwrap/texture2d.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

    class Shader;
    class IVW_MODULE_OPENGL_API ImageGL : public ImageRepresentation {

    public:
        ImageGL(uvec2 dimensions = uvec2(256,256), ImageType type = COLOR_DEPTH, const DataFormatBase* format = DataVec4UINT8::get(), 
            Texture2D* colorTexture = NULL, Texture2D* depthTexture = NULL, Texture2D* pickingTexture = NULL);
        virtual ~ImageGL();

        void initialize();
        void deinitialize();
        virtual DataRepresentation* clone() const;
        virtual std::string getClassName() const { return "ImageGL"; }

        void activateBuffer();
        void deactivateBuffer();

        void bindColorTexture(GLenum texUnit) const;
        void bindDepthTexture(GLenum texUnit) const;
        void bindPickingTexture(GLenum texUnit) const;
        void bindTextures(GLenum colorTexUnit, GLenum depthTexUnit) const;
        void unbindDepthTexture() const;
        void unbindColorTexture() const;
        void unbindPickingTexture() const;
        virtual void setDimensions(uvec2 dimensions);
        virtual void resize(uvec2 dimensions);
        virtual bool copyAndResizeImage(DataRepresentation*);
        bool updateFrom(const ImageGL*);

        FrameBufferObject* getFBO() {return frameBufferObject_;}
        const FrameBufferObject* getFBO() const {return frameBufferObject_;}

        Texture2D* getColorTexture() {return colorTexture_;}
        Texture2D* getDepthTexture() {return depthTexture_;}
        Texture2D* getPickingTexture() {return pickingTexture_;}
        const Texture2D* getColorTexture() const {return colorConstTexture_;}
        const Texture2D* getDepthTexture() const {return depthConstTexture_;}
        const Texture2D* getPickingTexture() const {return pickingConstTexture_;}

    protected:
        void useInputSource(ImageLayerType, const Image*);
        void createColorLayer();
        void createDepthLayer();
        void createPickingLayer();
        void createAndAddLayer(ImageLayerType);

        void invalidatePBOs();

    private:
        Texture2D* colorTexture_;
        Texture2D* depthTexture_;
        Texture2D* pickingTexture_;
        const Texture2D* colorConstTexture_;
        const Texture2D* depthConstTexture_;
        const Texture2D* pickingConstTexture_;
        FrameBufferObject* frameBufferObject_;
        GLenum pickingAttachmentID_;
        Shader* program_;
    };

} // namespace

#endif // IVW_IMAGEGL_H