#ifndef IVW_IMAGEOVERLAY_H
#define IVW_IMAGEOVERLAY_H

#include <modules/fontrendering/fontrenderingmoduledefine.h>
#include <modules/opengl/processorgl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace inviwo {
	class IVW_MODULE_FONTRENDERING_API ImageOverlay : public ProcessorGL {
public:
	ImageOverlay();
	~ImageOverlay();

	InviwoProcessorInfo();

	void initialize();
	void deinitialize();

protected:
	virtual void process();

private:
	FT_Library lib;
};
} // namespace

#endif // IVW_IMAGEOVERLAY_H