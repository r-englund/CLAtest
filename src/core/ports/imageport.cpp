#include "inviwo/core/ports/imageport.h"
#include "../../inviwo/modules/opengl/imagegl.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

    ImagePort::ImagePort(PortDirection direction, std::string identifier)
        : dimensions_(ivec2(256,256)),
          DataPort<Image>(direction, identifier)
    {}

    ImagePort::~ImagePort() {}

    void ImagePort::initialize() {
        data_ = new Image(dimensions_);
    }

    void ImagePort::deinitialize() {
        delete data_;
    }

    void ImagePort::resize(ivec2 dimensions) {
        dimensions_ = dimensions;
        data_->resize(dimensions_);
        invalidate();
    }

    Image* ImagePort::scaledData(Image* indata, Processor* inProcessor) {
        //This method should be called only from outport 

        ivec2 dim = indata->size();
        if ( (dimensions_[0] != dim[0]) || (dimensions_[1] != dim[1]) ) {

            Image* newData = 0;
            if (imageDataMap_.find(inProcessor->getIdentifier()) == imageDataMap_.end()) {
                newData = new Image();
                imageDataMap_.insert(std::make_pair(inProcessor->getIdentifier(), newData) ); 
                //std::cout<<"New Outport Data allocated in "<< getProcessor()->getIdentifier()<< " in image port "
                //         << getIdentifier() << " from processor "<< inProcessor->getIdentifier() <<"!!"<<std::endl;
            }
            else {
                //std::cout<<"Old Outport Data retrieved in "<< getProcessor()->getIdentifier()<< " in image port "
                //    << getIdentifier() << " from processor "<< inProcessor->getIdentifier() <<"!!"<<std::endl;
                newData = imageDataMap_[inProcessor->getIdentifier()];
            } 
            
            //TODO: ImageGL module should not be accessed from here
            ImageGL* newImageGL = newData->getRepresentation<ImageGL>();
            newData->resize(dim);

            ImageGL* imageGL = data_->getRepresentation<ImageGL>();
            imageGL->blit(newImageGL);

            invalidate();
            return newData;
        }
        return data_;
    }

} // namespace
