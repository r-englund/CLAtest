#ifndef IVW_IMAGERAM_H
#define IVW_IMAGERAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <inviwo/core/util/formats.h>

namespace inviwo {

class IVW_CORE_API ImageRAM : public ImageRepresentation {

public:
    ImageRAM();
    ImageRAM(uvec2 dimension);
    ImageRAM(uvec2 dimension, DataFormatBase format = DataFormatBase());

    virtual ~ImageRAM();

    virtual void initialize();
    void deinitialize();
    DataRepresentation* clone() const;
    virtual std::string getClassName() const { return "ImageRAM"; }
    void copyAndResizeImage(DataRepresentation*){}
    virtual void* getData() {return data_;};
    virtual const void* getData() const {return data_;};
    // Takes ownership of data pointer
    void setData(void* data) {
        delete[] data_; data_ = data;
    }
protected:
    void* data_;
};

template<typename T>
class IVW_CORE_API ImageRAMPrecision : public ImageRAM {
public:
    ImageRAMPrecision(uvec2 dimensions = uvec2(128,128), DataFormatBase format = GenericDataFormat(T)());
    ImageRAMPrecision(T* data, uvec2 dimensions = uvec2(128,128), DataFormatBase format = GenericDataFormat(T)());
    ImageRAMPrecision(const ImageRAMPrecision<T>& rhs) {
        *this = rhs;
    }
    ImageRAMPrecision<T>& operator=(const ImageRAMPrecision<T>& rhs) {
        if (this != &rhs) {
            delete[] data_;
            dimensions_ = rhs.getDimension();
            initialize();
            std::copy(rhs.getData(), rhs.getData()+dimensions_.x*dimensions_.y*sizeof(T), data_);
        }
        return *this;
    };
    virtual ~ImageRAMPrecision() {};
    virtual void initialize();
    virtual void initialize(void*);
    virtual DataRepresentation* clone() const;
};

template<typename T>
ImageRAMPrecision<T>::ImageRAMPrecision(uvec2 dimensions, DataFormatBase format) : ImageRAM(dimensions, format) {
    initialize();
}
template<typename T>
ImageRAMPrecision<T>::ImageRAMPrecision(T* data, uvec2 dimensions, DataFormatBase format) : ImageRAM(dimensions, format) {
    initialize(data);
}

template<typename T>
void ImageRAMPrecision<T>::initialize() {
    data_ = new T[dimensions_.x*dimensions_.y*sizeof(T)];
}

template<typename T>
void ImageRAMPrecision<T>::initialize(void* data) {
    if (data == NULL) {
        data_ = new T[dimensions_.x*dimensions_.y*sizeof(T)];
    } else {
        data_ = data;
    }
}

template<typename T>
DataRepresentation* ImageRAMPrecision<T>::clone() const {
    ImageRAMPrecision* newImageRAM = new ImageRAMPrecision<T>(getDimension());
    //*newImageRAM = *this;
    return newImageRAM;
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
IVW_CORE_API ImageRAM* createImage(const uvec2& dimension, const DataFormatBase& format);

} // namespace

#endif // IVW_IMAGERAM_H
