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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_VOLUMECLCONVERTER_H
#define IVW_VOLUMECLCONVERTER_H

#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramconverter.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <modules/opencl/volume/volumecl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeRAM2CLConverter : public RepresentationConverterType<VolumeCL> {

public:
    VolumeRAM2CLConverter();
    virtual ~VolumeRAM2CLConverter() {};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const VolumeRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API VolumeDisk2CLConverter : public RepresentationConverterPackage<VolumeCL> {

public:
    VolumeDisk2CLConverter() : RepresentationConverterPackage<VolumeCL>(){
        addConverter(new VolumeDisk2RAMConverter());
        addConverter(new VolumeRAM2CLConverter());
    };
    virtual ~VolumeDisk2CLConverter() {};
};

class IVW_MODULE_OPENCL_API VolumeCL2RAMConverter : public RepresentationConverterType<VolumeRAM> {

public:
    VolumeCL2RAMConverter();
    virtual ~VolumeCL2RAMConverter(){};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const VolumeCL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};



} // namespace

#endif // IVW_VOLUMECLCONVERTER_H
