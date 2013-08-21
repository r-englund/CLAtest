#ifndef IVW_IMAGEGL_H
#define IVW_IMAGEGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <modules/opengl/glwrap/framebufferobject.h>
#include <modules/opengl/glwrap/texture2d.h>

namespace inviwo {

    class Shader;
    class IVW_MODULE_OPENGL_API ImageGL : public ImageRepresentation {

    public:
        ImageGL(uvec2 dimensions = uvec2(256,256), ImageType type = COLOR_DEPTH, DataFormatBase format = DataVec4UINT8(), 
            Texture2D* colorTexture = NULL, Texture2D* depthTexture = NULL);
        virtual ~ImageGL();

        void initialize();
        void deinitialize();
        virtual DataRepresentation* clone() const;
        virtual std::string getClassName() const { return "ImageGL"; }
        void useInputSource(ImageLayerType, const Image*);

        void activateBuffer();
        void deactivateBuffer();

        void bindColorTexture(GLenum texUnit) const;
        void bindDepthTexture(GLenum texUnit) const;
        void bindTextures(GLenum colorTexUnit, GLenum depthTexUnit) const;
        void unbindDepthTexture() const;
        void unbindColorTexture() const;
        virtual void resize(uvec2 dimensions);
        virtual bool copyAndResizeImage(DataRepresentation* target);

        FrameBufferObject* getFBO() {return frameBufferObject_;}
        Texture2D* getColorTexture() {return colorTexture_;}
        Texture2D* getDepthTexture() {return depthTexture_;}
        const Texture2D* getColorTexture() const {return colorTexture_;}
        const Texture2D* getDepthTexture() const {return depthTexture_;}

    private:
        Texture2D* colorTexture_;
        Texture2D* depthTexture_;
        FrameBufferObject* frameBufferObject_;
    };

} // namespace

#endif // IVW_IMAGEGL_H