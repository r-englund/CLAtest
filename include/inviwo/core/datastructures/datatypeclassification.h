#ifndef IVW_DATA_TYPE_CLASSIFICATION_H
#define IVW_DATA_TYPE_CLASSIFICATION_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/data.h"
#include "inviwo/core/datastructures/volume.h"

namespace inviwo {

    template <typename T, typename U>
    class StructuredData : public U {
    public:
        StructuredData() {}
        virtual ~StructuredData(){}
        virtual Data* clone()=0;
    };    

    template <typename T, typename U>
    class UnStructuredData : public U {
    public:
        UnStructuredData(){}
        virtual ~UnStructuredData(){}
        virtual Data* clone()=0;
    };

    template <typename T, typename U>
    class RectiLinearData : public U {
    public:
        RectiLinearData(){}
        virtual ~RectiLinearData(){}
        virtual Data* clone()=0;
    }; 

    template <typename T, typename U>
    class CurviLinearData : public U {
    public:
        CurviLinearData(){}
        virtual ~CurviLinearData(){}
        virtual Data* clone()=0;
    };

    template <typename T, typename U>
    class UniformData : public U {
    public:
        UniformData(){}
        virtual ~UniformData(){}
        virtual Data* clone()=0;
    }; 

    template <typename T, typename U>
    class NonUniformData : public U {
    public:
        NonUniformData(){}
        virtual ~NonUniformData(){}
        virtual Data* clone()=0;
    };

} // namespace

#endif // IVW_DATA_TYPE_CLASSIFICATION_H
