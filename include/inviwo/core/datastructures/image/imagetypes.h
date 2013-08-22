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

static bool typeContainsColor(ImageType type){
    return (type == COLOR_ONLY || type == COLOR_DEPTH || type == COLOR_PICKING || type == COLOR_DEPTH_PICKING);
}

static bool typeContainsDepth(ImageType type){
    return (type == DEPTH_ONLY || type == COLOR_DEPTH || type == COLOR_DEPTH_PICKING);
}

static bool typeContainsPicking(ImageType type){
    return (type == COLOR_PICKING || type == COLOR_DEPTH_PICKING);
}

} // namespace

#endif // IVW_IMAGETYPES_H
