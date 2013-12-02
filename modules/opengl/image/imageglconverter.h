/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#ifndef IVW_IMAGEGLCONVERTER_H
#define IVW_IMAGEGLCONVERTER_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/image/imagegl.h>
#include <inviwo/core/datastructures/image/imageramconverter.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ImageRAM2GLConverter : public RepresentationConverterType<ImageGL> {

public:
    ImageRAM2GLConverter();
    virtual ~ImageRAM2GLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const ImageRAM*>(source) != NULL;
    }

    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENGL_API ImageGL2RAMConverter : public RepresentationConverterType<ImageRAM> {

public:
    ImageGL2RAMConverter();
    virtual ~ImageGL2RAMConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const ImageGL*>(source) != NULL;
    }

    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENGL_API ImageDisk2GLConverter : public RepresentationConverterPackage<ImageGL> {

public:
    ImageDisk2GLConverter() : RepresentationConverterPackage<ImageGL>(){
        addConverter(new ImageDisk2RAMConverter());
        addConverter(new ImageRAM2GLConverter());
    };
    virtual ~ImageDisk2GLConverter() {};
};

} // namespace

#endif // IVW_IMAGEGLCONVERTER_H
