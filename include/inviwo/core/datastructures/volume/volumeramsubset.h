#ifndef IVW_VOLUMERAMSUBSET_H
#define IVW_VOLUMERAMSUBSET_H

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramoperation.h>

namespace inviwo {
   
    class IVW_CORE_API VolumeRAMSubSet : public VolumeRAMOperation {
    public:
        VolumeRAMSubSet(const VolumeRepresentation* in, uvec3 dim, uvec3 offset, VolumeRepresentation::VolumeBorders border, bool clampBorderOutsideVolume) 
            : VolumeRAMOperation(in), newDim_(dim), newOffset_(offset), newBorder_(border), clampBorderOutsideVolume_(clampBorderOutsideVolume) {}
        virtual ~VolumeRAMSubSet() {}

        template<typename T, size_t B>
        void evaluate();

        static inline VolumeRAM* apply(const VolumeRepresentation* in, uvec3 dim, uvec3 offset, VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), bool clampBorderOutsideVolume = true){
            VolumeRAMSubSet subsetOP = VolumeRAMSubSet(in, dim, offset, border, clampBorderOutsideVolume);
            in->performOperation(&subsetOP);
            return subsetOP.getOutputVolume();
        }

    private:
        uvec3 newDim_;
        uvec3 newOffset_;
        VolumeRepresentation::VolumeBorders newBorder_;
        bool clampBorderOutsideVolume_;
    };

    template<typename T>
    class VolumeRAMPrecision;

    template<typename T, size_t B>
    class VolumeRAMCustomPrecision;

    template<typename T, size_t B>
    void VolumeRAMSubSet::evaluate(){
        const VolumeRAMPrecision<T>* volume = dynamic_cast<const VolumeRAMPrecision<T>*>(getInputVolume());
        if (!volume){
            setOutputVolume(NULL);
            return;
        }

        uvec3 dataDims = volume->getDimensions();
        if (newOffset_.x > dataDims.x && newOffset_.y > dataDims.y && newOffset_.z > dataDims.z){
            setOutputVolume(NULL);
            return;
        }

        // determine parameters
        uvec3 copyDataDims = static_cast<uvec3>(glm::max(static_cast<ivec3>(newDim_) - glm::max(static_cast<ivec3>(newOffset_+newDim_)-static_cast<ivec3>(dataDims), ivec3(0,0,0)), ivec3(0,0,0)));
        ivec3 newOffset_Dims = static_cast<ivec3>(glm::min(newOffset_, dataDims)-newBorder_.llf);
        VolumeRepresentation::VolumeBorders trueBorder = VolumeRepresentation::VolumeBorders();
        VolumeRepresentation::VolumeBorders correctBorder = newBorder_;

        if (clampBorderOutsideVolume_){
            correctBorder.llf += static_cast<uvec3>(-glm::min(newOffset_Dims, ivec3(0,0,0)));
            correctBorder.urb += static_cast<uvec3>(-glm::min(static_cast<ivec3>(dataDims)-static_cast<ivec3>(newOffset_+copyDataDims+correctBorder.urb), ivec3(0,0,0)));
            newOffset_Dims = static_cast<ivec3>(newOffset_-correctBorder.llf);
        }
        else{
            trueBorder.llf = static_cast<uvec3>(-glm::min(newOffset_Dims, ivec3(0,0,0)));
            trueBorder.urb = static_cast<uvec3>(glm::max(static_cast<ivec3>(newOffset_+copyDataDims+correctBorder.urb)-static_cast<ivec3>(dataDims),ivec3(0,0,0)));
        }

        uvec3 newOffset_DimsU = static_cast<uvec3>(glm::max(newOffset_Dims, ivec3(0,0,0)));
        size_t initialStartPos = (newOffset_DimsU.z * (dataDims.x*dataDims.y))+(newOffset_DimsU.y * dataDims.x) + newOffset_DimsU.x;
        uvec3 dimsWithBorder = newDim_+correctBorder.llf+correctBorder.urb;
        uvec3 copyDimsWithoutBorder = static_cast<uvec3>(glm::max(static_cast<ivec3>(copyDataDims+correctBorder.llf+correctBorder.urb)-static_cast<ivec3>(trueBorder.llf)-static_cast<ivec3>(trueBorder.urb), ivec3(1,1,1)));

        // per row
        size_t dataSize = copyDimsWithoutBorder.x*static_cast<size_t>(volume->getDataFormat()->getBytesAllocated());

        //allocate space
        VolumeRAMPrecision<T>* newVolume;
        if (volume->getDataFormat()->getBitsAllocated() != B)
            newVolume = new VolumeRAMCustomPrecision<T, B>(newDim_, correctBorder);
        else
            newVolume = new VolumeRAMPrecision<T>(newDim_, correctBorder);

        //newVolume->clear();
        const T* src = reinterpret_cast<const T*>(volume->getData());
        T* dst = reinterpret_cast<T*>(newVolume->getData());

        // memcpy each row for every slice to form sub volume
        size_t volumePos;
        size_t subVolumePos;
        for (size_t i=0; i < copyDimsWithoutBorder.z; i++) {
            for (size_t j=0; j < copyDimsWithoutBorder.y; j++) {
                volumePos = (j*dataDims.x) + (i*dataDims.x*dataDims.y);
                subVolumePos = ((j+trueBorder.llf.y)*dimsWithBorder.x) + ((i+trueBorder.llf.z)*dimsWithBorder.x*dimsWithBorder.y) + trueBorder.llf.x;
                memcpy(dst + subVolumePos, (src + volumePos + initialStartPos), dataSize);
            }
        }

        setOutputVolume(newVolume);
    }

} // namespace

#endif // IVW_VOLUMERAMSUBSET_H
