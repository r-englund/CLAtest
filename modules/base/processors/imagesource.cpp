#include "imagesource.h"

namespace inviwo {

ImageSource::ImageSource()
    : ProcessorGL(),
    outport_(Port::OUTPORT, "image.outport"),
    imageFileName_("imageFileName", "Image file name", IVW_DIR+"data/images/swirl.tga")
{
    addPort(outport_);
    addProperty(imageFileName_);
}

ImageSource::~ImageSource() {}

Processor* ImageSource::create() const {
    return new ImageSource();
}

void ImageSource::initialize() {
    ProcessorGL::initialize();

	Image* image = outport_.getData();
	if(image)
		image->addRepresentation(new ImageDisk(imageFileName_.get()));
    shader_ = new Shader("img_texturequad.frag");
}

void ImageSource::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void ImageSource::process() {
    activateTarget(outport_);

	Image* outImage = outport_.getData();
	
	ImageGL *outImage_ = outImage->getRepresentation<ImageGL>();

	ImageDisk *outImageDisk_ = outImage->getRepresentation<ImageDisk>();
	outImageDisk_ = new ImageDisk(imageFileName_.get());
	outImageDisk_->initialize();
    outImageDisk_->loadFileData();

	//uvec2 csize = outImage->size();*/

	uvec2 csize = outImage_->getDimension();

	glActiveTexture(GL_TEXTURE0);
	Texture2D* texture = ImageLoader::loadImage(imageFileName_.get());

	if(texture != NULL){

	//	outImage_->resize(uvec2(texture->getWidth(), texture->getHeight()));

		shader_->activate();
		shader_->setUniform("colorTex_", 0);
		shader_->setUniform("dimension_", vec2(1.f/csize[0],  1.f/csize[1]));
		renderImagePlaneQuad();
		shader_->deactivate();
		
		outImage->addRepresentation(outImageDisk_);

	}
	else{
		//Print in console if the image did not load correctly.
		LogInfoS(getClassName(), "Error: The specified image file was not loaded " 
								<< "correctly. The file format might not be supported.");
	}
	deactivateCurrentTarget();
}

} // namespace
