#ifndef IVW_IMAGERAM_H
#define IVW_IMAGERAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <inviwo/core/util/formats.h>

namespace inviwo {

class IVW_CORE_API ImageRAM : public ImageRepresentation {

public:
    ImageRAM(uvec2 dimension, ImageType type, const DataFormatBase* format = DataFormatBase::get());

    virtual ~ImageRAM();

    virtual void initialize();
    virtual void deinitialize();
    DataRepresentation* clone() const = 0;
    virtual std::string getClassName() const { return "ImageRAM"; }
    virtual void resize(uvec2 dimensions);
    virtual bool copyAndResizeImage(DataRepresentation*);
    virtual void* getData() {return data_;};
    virtual const void* getData() const {return data_;};
    virtual vec4 getValueAsVec4Float(const uvec2& pos) const = 0;

    // Takes ownership of data pointer
    void setData(void* data) {
        deinitialize();
        data_ = data;
    }
protected:
    void* data_;
};

/**
 * Factory for images. 
 * Creates an ImageRAM with data type specified by format. 
 * 
 * @param dimension of image to create.
 * @param format of image to create.
 * @return NULL if no valid format was specified. 
 */
IVW_CORE_API ImageRAM* createImageRAM(const uvec2& dimension, ImageType type, const DataFormatBase* format);

} // namespace

#endif // IVW_IMAGERAM_H
