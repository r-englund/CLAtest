#ifndef IVW_IMAGEOVERLAY_H
#define IVW_IMAGEOVERLAY_H

#include <modules/fontrendering/fontrenderingmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <inviwo/core/properties/stringproperty.h>
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

	void render_text(const char *text, float x, float y, float sx, float sy);

protected:
	virtual void process();

private:
	ImageInport inport0_;
	ImageOutport outport_;
	FT_Library fontlib_;
	FT_Face fontface_;
	StringProperty textStringProperty_;
	FloatVec4Property floatColor_;
	OptionPropertyInt optionPropertyIntFontSize_;
	FloatVec2Property floatVec2FontPos_;

	unsigned int font_size_;
	float xpos_;
	float ypos_;

	Shader* shader_;
	Shader* shader_passthrough_;
};

} // namespace

#endif // IVW_IMAGEOVERLAY_H