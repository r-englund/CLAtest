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

#ifndef IVW_LAYERCLCONVERTER_H
#define IVW_LAYERCLCONVERTER_H

#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/image/layerramconverter.h> 
#include <inviwo/core/datastructures/image/layerdisk.h>
#include <modules/opengl/image/layerglconverter.h> 
#include <modules/opencl/image/layercl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API LayerRAM2CLConverter : public RepresentationConverterType<LayerCL> {

public:
    LayerRAM2CLConverter();
    virtual ~LayerRAM2CLConverter() {};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const LayerRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API LayerDisk2CLConverter : public RepresentationConverterPackage<LayerCL> {

public:
    LayerDisk2CLConverter() : RepresentationConverterPackage<LayerCL>() {
        addConverter(new LayerDisk2RAMConverter());
        addConverter(new LayerRAM2CLConverter());
    };
    virtual ~LayerDisk2CLConverter() {};
};

class IVW_MODULE_OPENCL_API LayerCL2RAMConverter : public RepresentationConverterType<LayerRAM> {

public:
    LayerCL2RAMConverter();
    virtual ~LayerCL2RAMConverter(){};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const LayerCL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API LayerGL2CLConverter : public RepresentationConverterPackage<LayerCL> {

public:
    LayerGL2CLConverter() : RepresentationConverterPackage<LayerCL>() {
        addConverter(new LayerGL2RAMConverter());
        addConverter(new LayerRAM2CLConverter());
    };
    virtual ~LayerGL2CLConverter() {};
};

class IVW_MODULE_OPENCL_API LayerCL2GLConverter : public RepresentationConverterPackage<LayerGL> {

public:
    LayerCL2GLConverter() : RepresentationConverterPackage<LayerGL>() {
        addConverter(new LayerCL2RAMConverter());
        addConverter(new LayerRAM2GLConverter());
    };
    virtual ~LayerCL2GLConverter() {};
};



} // namespace

#endif // IVW_LAYERCLCONVERTER_H
