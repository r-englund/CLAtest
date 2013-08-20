#ifndef IVW_IMAGETYPES_H
#define IVW_IMAGETYPES_H

namespace inviwo {

    enum ImageType{
        COLOR_ONLY,
        DEPTH_ONLY,
        PICKING_ONLY,
        COLOR_DEPTH,
        COLOR_PICKING,
        COLOR_DEPTH_PICKING
    };

    enum ImageLayerType{
        COLOR_LAYER,
        DEPTH_LAYER,
        PICKING_LAYER
    };

} // namespace

#endif // IVW_IMAGETYPES_H
