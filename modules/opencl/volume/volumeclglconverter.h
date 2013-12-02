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

#ifndef IVW_VOLUMECLGLCONVERTER_H 
#define IVW_VOLUMECLGLCONVERTER_H

#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramconverter.h> 
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <modules/opengl/volume/volumeglconverter.h> 
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/volume/volumecl.h>
#include <modules/opencl/volume/volumeclgl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeRAM2CLGLConverter : public RepresentationConverterPackage<VolumeCLGL> {

public:
    VolumeRAM2CLGLConverter();
    virtual ~VolumeRAM2CLGLConverter() {};
};



class IVW_MODULE_OPENCL_API VolumeCLGL2RAMConverter : public RepresentationConverterType<VolumeRAM> {

public:
    VolumeCLGL2RAMConverter();
    virtual ~VolumeCLGL2RAMConverter(){};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const VolumeCLGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API VolumeGL2CLGLConverter : public RepresentationConverterType<VolumeCLGL> {

public:
    VolumeGL2CLGLConverter() : RepresentationConverterType<VolumeCLGL>() {};
    virtual ~VolumeGL2CLGLConverter() {};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const VolumeGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API VolumeCLGL2CLConverter : public RepresentationConverterType<VolumeCL> {
public:
    VolumeCLGL2CLConverter() : RepresentationConverterType<VolumeCL>() {};
    virtual ~VolumeCLGL2CLConverter() {};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const VolumeCLGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API VolumeCL2CLGLConverter : public RepresentationConverterPackage<VolumeCLGL> {
public:
    VolumeCL2CLGLConverter();
    virtual ~VolumeCL2CLGLConverter() {};
};

class IVW_MODULE_OPENCL_API VolumeDisk2CLGLConverter : public RepresentationConverterPackage<VolumeCLGL> {

public:
    VolumeDisk2CLGLConverter() : RepresentationConverterPackage<VolumeCLGL>() {
        addConverter(new VolumeDisk2RAMConverter());
        addConverter(new VolumeRAM2GLConverter());
        addConverter(new VolumeGL2CLGLConverter());
    };
    virtual ~VolumeDisk2CLGLConverter() {};
};



} // namespace

#endif // IVW_VOLUMECLGLCONVERTER_H
