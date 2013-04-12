#ifndef IVW_IMAGERAM_H
#define IVW_IMAGERAM_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/imagerepresentation.h>
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
        using ImageRAM::initialize;
        virtual void initialize(void*);
        virtual void deinitialize();
        virtual DataRepresentation* clone() const;
    };

    template<typename T>
    ImageRAMPrecision<T>::ImageRAMPrecision(uvec2 dimensions, DataFormatBase format) : ImageRAM(dimensions, format) {
        initialize(0);
    }
    template<typename T>
    ImageRAMPrecision<T>::ImageRAMPrecision(T* data, uvec2 dimensions, DataFormatBase format) : ImageRAM(dimensions, format) {
        initialize(data);
    }

    template<typename T>
    void ImageRAMPrecision<T>::initialize(void* data) {
        if (!data)
            data_ = new T[dimensions_.x*dimensions_.y*sizeof(T)];
        else
            data_ = data;
        ImageRAM::initialize();
    }

    template<typename T>
    DataRepresentation* ImageRAMPrecision<T>::clone() const {
        ImageRAMPrecision* newImageRAM = new ImageRAMPrecision<T>(dimensions_);
        return newImageRAM;
    }

    template<typename T>
    void ImageRAMPrecision<T>::deinitialize() {
        delete static_cast<T*>(data_);
        data_ = 0;
        ImageRAM::deinitialize();
    }

    typedef ImageRAMPrecision<DataUINT8::type>     ImageRAMuint8;
    typedef ImageRAMPrecision<DataINT8::type>      ImageRAMint8;
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

} // namespace

#endif // IVW_IMAGERAM_H
