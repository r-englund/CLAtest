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

#ifndef IVW_BUFFERCL_CONVERTER_H
#define IVW_BUFFERCL_CONVERTER_H

#include <inviwo/core/common/inviwo.h>
#include <modules/opencl/buffer/buffercl.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opengl/buffer/bufferglconverter.h>

namespace inviwo {


class IVW_MODULE_OPENCL_API BufferRAM2CLConverter : public RepresentationConverterType<BufferCL> {

public:
    BufferRAM2CLConverter();
    virtual ~BufferRAM2CLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API BufferCL2RAMConverter : public RepresentationConverterType<BufferRAM> {

public:
    BufferCL2RAMConverter();
    virtual ~BufferCL2RAMConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferCL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API BufferGL2CLConverter : public RepresentationConverterPackage<BufferCL> {

public:
    BufferGL2CLConverter() : RepresentationConverterPackage<BufferCL>() {
        addConverter(new BufferGL2RAMConverter());
        addConverter(new BufferRAM2CLConverter());
    };
    virtual ~BufferGL2CLConverter() {};
};

class IVW_MODULE_OPENCL_API BufferCL2GLConverter : public RepresentationConverterPackage<BufferGL> {

public:
    BufferCL2GLConverter() : RepresentationConverterPackage<BufferGL>() {
        addConverter(new BufferCL2RAMConverter());
        addConverter(new BufferRAM2GLConverter());
    };
    virtual ~BufferCL2GLConverter() {};
};


} // namespace inviwo

#endif // IVW_BUFFERCL_CONVERTER_H