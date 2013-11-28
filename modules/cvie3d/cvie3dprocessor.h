#ifndef IVW_CVIE3DPROCESSOR_H
#define IVW_CVIE3DPROCESSOR_H

#include <modules/cvie3d/cvie3dmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <CVIE3D.h>

namespace inviwo {

class IVW_MODULE_CVIE3D_API CVIE3DProcessor : public Processor {
public:
    CVIE3DProcessor();
    ~CVIE3DProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();
    void passthrough();

    bool createCVIE3DInstance();
    void destroyCVIE3DInstance();

    bool setupEnhancement();
    bool runEnhancement();

    void updateConfigurationFile();
    void updateParameterFile();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    BoolProperty enabled_;
    FileProperty confFile_;
    FileProperty parameterFile_;
    IntProperty parameterSetting_;

    HCVIE3D cvieHandle_;
    bool cvieContextCreated_;
};

} // namespace

#endif // IVW_CVIE3DPROCESSOR_H
