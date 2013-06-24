#include "imageoverlay.h"

namespace inviwo {

ProcessorClassName(ImageOverlay, "ImageOverlay"); 
ProcessorCategory(ImageOverlay, "Compositor");
ProcessorCodeState(ImageOverlay, CODE_STATE_EXPERIMENTAL);

ImageOverlay::ImageOverlay() 
	: ProcessorGL(),
	inport0_("inport0"),
	outport_("outport"),
	textStringProperty_("Text","Text","Lorem ipsum etc.",PropertyOwner::INVALID_OUTPUT,PropertySemantics::Editor),
	font_size_(20),
	xpos_(0),
	ypos_(0),
	floatColor_("FloatColor","FloatColor", vec4(0.2),vec4(0.0),vec4(0.1),vec4(1.0),PropertyOwner::INVALID_OUTPUT,PropertySemantics::Color),
	optionPropertyIntFontSize_("Font size","Font size"),
	floatVec2FontPos_("Position","Position",glm::vec2(0.0f))
{
	addPort(inport0_);
	addPort(outport_);
	addProperty(textStringProperty_);
	addProperty(floatColor_);
	addProperty(floatVec2FontPos_);
	floatVec2FontPos_.setMinValue(glm::vec2(0.0f));
	floatVec2FontPos_.setMaxValue(glm::vec2(1.0f));
	addProperty(optionPropertyIntFontSize_);
	optionPropertyIntFontSize_.addOption("10","10",10);
	optionPropertyIntFontSize_.addOption("12","12",12);
	optionPropertyIntFontSize_.addOption("18","18",18);
	optionPropertyIntFontSize_.addOption("24","24",24);
	optionPropertyIntFontSize_.addOption("36","36",36);
	optionPropertyIntFontSize_.addOption("48","48",48);
	optionPropertyIntFontSize_.addOption("60","60",60);
	optionPropertyIntFontSize_.addOption("72","72",72);
	optionPropertyIntFontSize_.setSelectedOption(3);
}

ImageOverlay::~ImageOverlay() {}

void ImageOverlay::initialize() {
	ProcessorGL::initialize();
	shader_ = new Shader("fontrendering_freetype.vert", "fontrendering_freetype.frag",true);
	shader_passthrough_ = new Shader("fontrendering_passthrough.frag",true);

	int error = 0;
	if( FT_Init_FreeType(&fontlib_) )
		std::cout << "Major FT error.\n";

	error = FT_New_Face( fontlib_, 
		"../modules/fontrendering/fonts/arial.ttf",
		0,
		&fontface_);

	if(error == FT_Err_Unknown_File_Format)
		std::cout << "FT2 File opened and read, format unsupported.\n";
	else if(error)
		std::cout << "FT2 Could not read/open the font file.\n";
}
void ImageOverlay::deinitialize() {
	delete shader_passthrough_;
	delete shader_;
	ProcessorGL::deinitialize();
}

void ImageOverlay::render_text(const char *text, float x, float y, float sx, float sy){
	const char *p;
	FT_Set_Pixel_Sizes(fontface_, 0, optionPropertyIntFontSize_.get());

	for(p = text; *p; p++) {
		if( FT_Load_Char(fontface_, *p, FT_LOAD_RENDER) ) {
			std::cout << "FT could not render char\n";
			continue;
		}

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			fontface_->glyph->bitmap.width,
			fontface_->glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			fontface_->glyph->bitmap.buffer
			);

		float x2 = x + fontface_->glyph->bitmap_left * sx;
		float y2 = -y - fontface_->glyph->bitmap_top * sy;
		float w = fontface_->glyph->bitmap.width * sx;
		float h = fontface_->glyph->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{x2,	 -y2,	  0, 0},
			{x2 + w, -y2,	  1, 0},
			{x2,	 -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		shader_->setUniform("texture", 0);
		shader_->setUniform("color", floatColor_.get());
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (fontface_->glyph->advance.x >> 6) * sx;
		y += (fontface_->glyph->advance.y >> 6) * sy;
	}
}

void ImageOverlay::process() {
	
	const Image* inputImage = inport0_.getData();
	Image* outImage = outport_.getData();

	const ImageGL* inImageGL = inputImage->getRepresentation<ImageGL>();
	ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();

	uvec2 imageSize = inImageGL->getDimension();
	outImageGL->resize(imageSize);

	activateTarget(outport_);
	bindColorTexture(inport0_, GL_TEXTURE0);

	shader_passthrough_->activate();
	shader_passthrough_->setUniform("inport0_",0);
	shader_passthrough_->setUniform("dimension_", vec2(1.f / imageSize[0], 1.f / imageSize[1]) );
	renderImagePlaneQuad();
	shader_passthrough_->deactivate();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	GLuint attribute_location = 0;
	glEnableVertexAttribArray(attribute_location);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_location, 4, GL_FLOAT, GL_FALSE, 0, 0);

	float sx = 2.0 / imageSize[0];
	float sy = 2.0 / imageSize[1];
	font_size_ = optionPropertyIntFontSize_.getValue();
	xpos_ = floatVec2FontPos_.get().x * imageSize[0];
	ypos_ = floatVec2FontPos_.get().y * imageSize[1] + float(font_size_);

	shader_->activate();
	render_text(
		textStringProperty_.get().c_str(),
		-1 + xpos_ * sx,
		1 - (ypos_) * sy,
		sx,
		sy);
	shader_->deactivate();
	
	deactivateCurrentTarget();

}

} // namespace