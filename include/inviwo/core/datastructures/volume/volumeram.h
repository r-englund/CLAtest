#ifndef IVW_VOLUMERAM_H
#define IVW_VOLUMERAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

class IVW_CORE_API VolumeRAM : public VolumeRepresentation {

public:
    VolumeRAM(uvec3 dimension = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = DataFormatBase());
    virtual ~VolumeRAM();
    virtual void performOperation(DataOperation*) const = 0;
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const = 0;
    virtual void* getData();
    virtual const void* getData() const;
    virtual void saveData(std::string url) const;
    /** 
     * \brief Takes ownership of data pointer
     * 
     * @param void * data is raw volume data pointer
     * @return void none
     */
    void setData(void* data) {
        delete[] data_; 
        data_ = data;
    }
protected:
    void* data_;
};


/**
 * Factory for volumes.
 * Creates an VolumeRAM with data type specified by format. 
 * 
 * @param dimension of volume to create.
 * @param format of volume to create.
 * @return NULL if no valid format was specified.
 */
IVW_CORE_API VolumeRAM* createVolumeRAM(const uvec3& dimension, const DataFormatBase& format);

} // namespace

#endif // IVW_VOLUMERAM_H
