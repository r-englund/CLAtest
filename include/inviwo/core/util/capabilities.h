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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_CAPABILITIES_H
#define IVW_CAPABILITIES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

    class IVW_CORE_API Capabilities  {

    public:
        Capabilities (){}
        virtual ~Capabilities (){}

        virtual void initialize(){
            retrieveStaticInfo();
        }

        virtual void printInfo() = 0;

        virtual bool canAllocate(uint64_t dataSize, uint8_t percentageOfAvailableMemory = 100){
            return true;
        }
    
        virtual uvec3 calculateOptimalBrickSize(uvec3 dimensions, size_t formatSizeInBytes, uint8_t percentageOfAvailableMemory = 100){
            return dimensions;
        }

    protected:
        virtual uint64_t getMemorySizeInBytes(uvec3 dimensions, size_t formatSizeInBytes){
            return static_cast<uint64_t>(dimensions.x*dimensions.y*dimensions.z*formatSizeInBytes);
        }

        virtual void retrieveStaticInfo() = 0;
        virtual void retrieveDynamicInfo() = 0;
    };

} // namespace

#endif //IVW_CAPABILITIES_H
