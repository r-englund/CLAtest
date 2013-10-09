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
    void* getData() {return data_;};
    const void* getData() const {return data_;};
    void* getPickingData();

    virtual void setValueFromSingleFloat(const uvec2& pos, float val) = 0;
    virtual void setValueFromVec2Float(const uvec2& pos, vec2 val) = 0;
    virtual void setValueFromVec3Float(const uvec2& pos, vec3 val) = 0;
    virtual void setValueFromVec4Float(const uvec2& pos, vec4 val) = 0;

    virtual float getValueAsSingleFloat(const uvec2& pos) const = 0;
    virtual vec2 getValueAsVec2Float(const uvec2& pos) const = 0;
    virtual vec3 getValueAsVec3Float(const uvec2& pos) const = 0;
    virtual vec4 getValueAsVec4Float(const uvec2& pos) const = 0;

    virtual vec4 getPickingValue(const uvec2& pos) const = 0;

    // Takes ownership of data pointer
    void setData(void* data) {
        deinitialize();
        data_ = data;
    }

    static inline unsigned int posToIndex(const uvec2& pos, const uvec2& dim){
        return pos.x+(pos.y*dim.x);
    }

protected:
    virtual void allocatePickingData() = 0;

    void* data_;
    void* pickingData_;
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
