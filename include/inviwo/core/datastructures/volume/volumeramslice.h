#ifndef IVW_VOLUMERAMSLICE_H
#define IVW_VOLUMERAMSLICE_H

#include <inviwo/core/datastructures/image/imageramprecision.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeoperation.h>

namespace inviwo {
   
class IVW_CORE_API VolumeRAMSlice : public VolumeOperation {
public:
    /// Which plane to take slice from
    enum CoordinatePlane {
        XY,
        XZ,
        YZ
    };

    VolumeRAMSlice(const VolumeRepresentation* in, CoordinatePlane cPlane, unsigned int sliceNum) 
        : VolumeOperation(in), cPlane_(cPlane), sliceNum_(sliceNum) {}
    virtual ~VolumeRAMSlice() {}

    template<typename T, size_t B>
    void evaluate();

    static inline ImageRAM* apply(const VolumeRepresentation* in, CoordinatePlane cPlane, unsigned int sliceNum){
        VolumeRAMSlice sliceOP = VolumeRAMSlice(in, cPlane, sliceNum);
        in->performOperation(&sliceOP);
        return sliceOP.getOutput<ImageRAM>();
    }

private:
    CoordinatePlane cPlane_;
    unsigned int sliceNum_;
};

template<typename T>
class VolumeRAMPrecision;

template<typename T, size_t B>
class VolumeRAMCustomPrecision;

template<typename T, size_t B>
void VolumeRAMSlice::evaluate(){
    const VolumeRAMPrecision<T>* volume = dynamic_cast<const VolumeRAMPrecision<T>*>(getInputVolume());
    if (!volume){
        setOutput(NULL);
        return;
    }

    uvec3 dataDims = volume->getDimensions();
    if(cPlane_ == XY){ //XY Plane
        if (sliceNum_ >= dataDims.z){
            setOutput(NULL);
            return;
        }

        //allocate space
        ImageRAMPrecision<T>* sliceImage;
        if (volume->getDataFormat()->getBitsAllocated() != B)
            sliceImage = new ImageRAMCustomPrecision<T, B>(dataDims.xy(),  COLOR_ONLY);
        else
            sliceImage = new ImageRAMPrecision<T>(dataDims.xy(),  COLOR_ONLY);

        const T* src = reinterpret_cast<const T*>(volume->getData());
        T* dst = reinterpret_cast<T*>(sliceImage->getData());

        //copy data
        size_t dataSize = dataDims.x*static_cast<size_t>(volume->getDataFormat()->getBytesAllocated());
        size_t initialStartPos = sliceNum_*dataDims.x*dataDims.y;
        size_t offset;
        for (size_t j=0; j < dataDims.y; j++) {
            offset = (j*dataDims.x);
            memcpy(dst + offset, (src + offset + initialStartPos), dataSize);
        }
        setOutput(sliceImage);
    }
    else if(cPlane_ == XZ){ //XZ Plane
        if (sliceNum_ >= dataDims.y){
            setOutput(NULL);
            return;
        }

        //allocate space
        ImageRAMPrecision<T>* sliceImage;
        if (volume->getDataFormat()->getBitsAllocated() != B)
            sliceImage = new ImageRAMCustomPrecision<T, B>(dataDims.xz(),  COLOR_ONLY);
        else
            sliceImage = new ImageRAMPrecision<T>(dataDims.xz(),  COLOR_ONLY);

        const T* src = reinterpret_cast<const T*>(volume->getData());
        T* dst = reinterpret_cast<T*>(sliceImage->getData());

        //copy data
        size_t dataSize = dataDims.x*static_cast<size_t>(volume->getDataFormat()->getBytesAllocated());
        size_t initialStartPos = sliceNum_*dataDims.x;
        size_t offsetVolume;
        size_t offsetÍmage;
        for (size_t j=0; j < dataDims.z; j++) {
            offsetVolume = (j*dataDims.x*dataDims.y) + initialStartPos;
            offsetÍmage = j*dataDims.x;
            memcpy(dst + offsetÍmage, src + offsetVolume, dataSize);
        }
        setOutput(sliceImage);
    }
    else{ //YZ Plane
        if (sliceNum_ >= dataDims.x){
            setOutput(NULL);
            return;
        }

        //allocate space
        ImageRAMPrecision<T>* sliceImage;
        if (volume->getDataFormat()->getBitsAllocated() != B)
            sliceImage = new ImageRAMCustomPrecision<T, B>(dataDims.yz(),  COLOR_ONLY);
        else
            sliceImage = new ImageRAMPrecision<T>(dataDims.yz(),  COLOR_ONLY);

        const T* src = reinterpret_cast<const T*>(volume->getData());
        T* dst = reinterpret_cast<T*>(sliceImage->getData());

        //copy data
        /*size_t offsetVolume;
        size_t offsetÍmage;
        for (size_t i=0; i < dataDims.z; i++) {
            for (size_t j=0; j < dataDims.y; j++) {
                offsetÍmage = (i*dataDims.z)+j;
                dst[offsetÍmage] = src[];
            }
        }*/
        setOutput(sliceImage);
    }
}

} // namespace

#endif // IVW_VOLUMERAMSLICE_H
