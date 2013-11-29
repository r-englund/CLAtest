#ifndef IVW_TEXTURE_OBSERVER_H
#define IVW_TEXTURE_OBSERVER_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/observer.h>


namespace inviwo {

/** \class TextureObserver 
*
* This observer is notified before and after 
* a texture is initialized (glTexture is called).
* This enables shared objects to release and rebind the texture.
* 
*
* @see Observable
* @see Texture2D
* @see Texture3D
*/
class IVW_MODULE_OPENGL_API TextureObserver: public Observer {
public:
    TextureObserver(): Observer() {};

    /**
    * This method will be called before the texture is initialized.
    * Override it to add behavior.
    */
    virtual void notifyBeforeTextureInitialization() {};

    /**
    * This method will be called after the texture has been initialized.
    * Override it to add behavior.
    */
    virtual void notifyAfterTextureInitialization() {};
};


} // namespace

#endif // IVW_TEXTURE_OBSERVER_H
