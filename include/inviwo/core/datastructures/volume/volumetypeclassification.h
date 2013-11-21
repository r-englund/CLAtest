#ifndef IVW_VOLUME_TYPE_CLASSIFICATION_H
#define IVW_VOLUME_TYPE_CLASSIFICATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/datatypeclassification.h>

namespace inviwo {
   
    //TODO: two ways to derive inheritance typedefs or class inheritance
    //      ( typedefs doesn't allow to see class hierarchy especially during documentation)

    /*
    typedef StructuredData<Data::TYPE3D, Volume> StructuredVolume;
    typedef RectiLinearData<Data::TYPE3D, StructuredVolume> RectiLinearVolume;
    typedef CurviLinearData<Data::TYPE3D, StructuredVolume> CurviLinearVolume;

    typedef UniformData<Data::TYPE3D, RectiLinearVolume> UniformRectiLinearVolume;    
    typedef NonUniformData<Data::TYPE3D, RectiLinearVolume> NonUniformRectiLinearVolume;
    typedef UniformData<Data::TYPE3D, CurviLinearVolume> UniformCurviLinearVolume;
    typedef NonUniformData<Data::TYPE3D, CurviLinearVolume> NonUniformCurviLinearVolume;
    */

    //TODO: So for now use classes are used instead of typedefs

    class IVW_CORE_API RectiLinearVolume : public RectiLinearData<3, Volume> {  
    public:
        RectiLinearVolume() {}
        ~RectiLinearVolume() {}
        virtual Data* clone()=0;
    };

    class IVW_CORE_API CurviLinearVolume : public CurviLinearData<3, Volume> {
    public:
        CurviLinearVolume() {}
        ~CurviLinearVolume() {}
        virtual Data* clone()=0;
    };

    /*----------------------------------------------------------------------*/

    class IVW_CORE_API UniformRectiLinearVolume : public UniformData<3, RectiLinearVolume> {
    public:
        UniformRectiLinearVolume();
        ~UniformRectiLinearVolume();
        virtual Data* clone();
    };

    class IVW_CORE_API UniformCurviLinearVolume : public UniformData<3, CurviLinearVolume> {
    public:
        UniformCurviLinearVolume();
        ~UniformCurviLinearVolume();
        virtual Data* clone();
    };

    class IVW_CORE_API NonUniformRectiLinearVolume : public NonUniformData<3, RectiLinearVolume> {
    public:
        NonUniformRectiLinearVolume();
        ~NonUniformRectiLinearVolume();
        virtual Data* clone();
    };

    class IVW_CORE_API NonUniformCurviLinearVolume : public NonUniformData<3, CurviLinearVolume> {
    public:
        NonUniformCurviLinearVolume();
        ~NonUniformCurviLinearVolume();
        virtual Data* clone();
    };

    typedef UniformRectiLinearVolume StandardVolume;
    
} // namespace

#endif // IVW_VOLUME_TYPE_CLASSIFICATION_H
