/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

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
