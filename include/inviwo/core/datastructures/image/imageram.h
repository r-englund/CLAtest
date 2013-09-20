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
    virtual glm::vec4 getValueAsVec4Float(const glm::uvec2& pos) const = 0;

    // Takes ownership of data pointer
    void setData(void* data) {
        deinitialize();
        data_ = data;
    }
protected:
    void* data_;
};

template<typename T>
class IVW_CORE_API ImageRAMPrecision : public ImageRAM {
public:
    ImageRAMPrecision(uvec2 dimensions = uvec2(128,128), ImageType type = COLOR_DEPTH, const DataFormatBase* format = GenericDataFormat(T)::get());
    ImageRAMPrecision(T* data, uvec2 dimensions = uvec2(128,128), ImageType type = COLOR_DEPTH, const DataFormatBase* format = GenericDataFormat(T)::get());
    ImageRAMPrecision(const ImageRAMPrecision<T>& rhs): ImageRAM() {
        *this = rhs;
    }
    ImageRAMPrecision<T>& operator=(const ImageRAMPrecision<T>& rhs) {
        if (this != &rhs) {
            delete[] data_;
            dimensions_ = rhs.getDimensions();
            initialize();
            std::copy(rhs.getData(), rhs.getData()+dimensions_.x*dimensions_.y*sizeof(T), data_);
        }
        return *this;
    };
    virtual ~ImageRAMPrecision() {};
    virtual void initialize();
    virtual void initialize(void*);
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    virtual glm::vec4 getValueAsVec4Float(const glm::uvec2& pos) const;
};

template<typename T>
ImageRAMPrecision<T>::ImageRAMPrecision(uvec2 dimensions, ImageType type, const DataFormatBase* format) : ImageRAM(dimensions, type, format) {
    initialize();
}
template<typename T>
ImageRAMPrecision<T>::ImageRAMPrecision(T* data, uvec2 dimensions, ImageType type, const DataFormatBase* format) : ImageRAM(dimensions, type, format) {
    initialize(data);
}

template<typename T>
void ImageRAMPrecision<T>::initialize() {
    data_ = new T[dimensions_.x*dimensions_.y];
}

template<typename T>
void ImageRAMPrecision<T>::initialize(void* data) {
    if (data == NULL) {
        data_ = new T[dimensions_.x*dimensions_.y];
    } else {
        data_ = data;
    }
}

template<typename T>
void inviwo::ImageRAMPrecision<T>::deinitialize()
{
    if(data_) {
        delete[] static_cast<T*>(data_);
        data_ = NULL;
    }
}

template<typename T>
DataRepresentation* ImageRAMPrecision<T>::clone() const {
    ImageRAMPrecision* newImageRAM = new ImageRAMPrecision<T>(getDimensions());
    //*newImageRAM = *this;
    return newImageRAM;
}

template<typename T>
glm::vec4 ImageRAMPrecision<T>::getValueAsVec4Float(const uvec2& pos) const{
    glm::vec4 result;
    T* data = static_cast<T*>(data_);
    T val = data[pos.x+(pos.y*dimensions_.x)];
    result = getDataFormat()->convertToNormalizedVec4Float(&val);
    return result;
}

typedef ImageRAMPrecision<DataUINT8::type>     ImageRAMuint8;
typedef ImageRAMPrecision<DataINT8::type>      ImageRAMint8;
typedef ImageRAMPrecision<DataUINT12::type>    ImageRAMuint12;
typedef ImageRAMPrecision<DataINT12::type>     ImageRAMint12;
typedef ImageRAMPrecision<DataUINT16::type>    ImageRAMuint16;
typedef ImageRAMPrecision<DataINT16::type>     ImageRAMint16;
typedef ImageRAMPrecision<DataUINT32::type>    ImageRAMuint32;
typedef ImageRAMPrecision<DataUINT64::type>    ImageRAMuint64;
typedef ImageRAMPrecision<DataINT32::type>     ImageRAMint32;
typedef ImageRAMPrecision<DataFLOAT16::type>   ImageRAMfloat16;
typedef ImageRAMPrecision<DataFLOAT32::type>   ImageRAMfloat32;
typedef ImageRAMPrecision<DataFLOAT64::type>   ImageRAMfloat64;

typedef ImageRAMPrecision<DataVec4UINT8::type>      ImageRAMVec4uint8;
typedef ImageRAMPrecision<DataVec4FLOAT32::type>    ImageRAMVec4float32;


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
