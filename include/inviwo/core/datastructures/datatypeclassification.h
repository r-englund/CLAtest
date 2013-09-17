#ifndef IVW_DATA_TYPE_CLASSIFICATION_H
#define IVW_DATA_TYPE_CLASSIFICATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

    template <unsigned int N, typename U>
    class IVW_CORE_API RectiLinearData : public U {
    public:
        RectiLinearData(){}
        virtual ~RectiLinearData(){}
        virtual Data* clone()=0;
    }; 

    template <unsigned int N, typename U>
    class IVW_CORE_API CurviLinearData : public U {
    public:
        CurviLinearData(){}
        virtual ~CurviLinearData(){}
        virtual Data* clone()=0;
    };

    template <unsigned int N, typename U>
    class IVW_CORE_API UniformData : public U {
    public:
        UniformData(){}
        virtual ~UniformData(){}
        virtual Data* clone()=0;
    }; 

    template <unsigned int N, typename U>
    class IVW_CORE_API NonUniformData : public U {
    public:
        NonUniformData(){}
        virtual ~NonUniformData(){}
        virtual Data* clone()=0;
    };

} // namespace

#endif // IVW_DATA_TYPE_CLASSIFICATION_H
