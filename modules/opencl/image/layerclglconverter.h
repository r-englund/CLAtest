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

#ifndef IVW_LAYERCLGLCONVERTER_H 
#define IVW_LAYERCLGLCONVERTER_H

#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/image/layerramconverter.h> 
#include <modules/opengl/image/layerglconverter.h> 
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/image/layercl.h>
#include <modules/opencl/image/layerclgl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API LayerRAM2CLGLConverter : public RepresentationConverterPackage<LayerCLGL> {

public:
    LayerRAM2CLGLConverter();
    virtual ~LayerRAM2CLGLConverter() {};
};



class IVW_MODULE_OPENCL_API LayerCLGL2RAMConverter : public RepresentationConverterType<LayerRAM> {

public:
    LayerCLGL2RAMConverter();
    virtual ~LayerCLGL2RAMConverter(){};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const LayerCLGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API LayerCLGL2GLConverter : public RepresentationConverterType<LayerGL> {

public:
    LayerCLGL2GLConverter();
    virtual ~LayerCLGL2GLConverter(){};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const LayerCLGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API LayerGL2CLGLConverter : public RepresentationConverterType<LayerCLGL> {

public:
    LayerGL2CLGLConverter() : RepresentationConverterType<LayerCLGL>() {};
    virtual ~LayerGL2CLGLConverter() {};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const LayerGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API LayerCLGL2CLConverter : public RepresentationConverterType<LayerCL> {
public:
    LayerCLGL2CLConverter() : RepresentationConverterType<LayerCL>() {};
    virtual ~LayerCLGL2CLConverter() {};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const LayerCLGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API LayerCL2CLGLConverter : public RepresentationConverterPackage<LayerCLGL> {
public:
    LayerCL2CLGLConverter();
    virtual ~LayerCL2CLGLConverter() {};
};

class IVW_MODULE_OPENCL_API LayerDisk2CLGLConverter : public RepresentationConverterPackage<LayerCLGL> {

public:
    LayerDisk2CLGLConverter() : RepresentationConverterPackage<LayerCLGL>() {
        addConverter(new LayerDisk2RAMConverter());
        addConverter(new LayerRAM2GLConverter());
        addConverter(new LayerGL2CLGLConverter());
    };
    virtual ~LayerDisk2CLGLConverter() {};
};

} // namespace

#endif // IVW_LAYERCLGLCONVERTER_H
