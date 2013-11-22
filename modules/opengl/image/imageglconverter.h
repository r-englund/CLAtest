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
