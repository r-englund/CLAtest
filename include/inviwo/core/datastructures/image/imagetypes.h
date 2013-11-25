#ifndef IVW_IMAGETYPES_H
#define IVW_IMAGETYPES_H

namespace inviwo {

enum ImageType{
    COLOR_ONLY = 0,
    COLOR_DEPTH = 1,
    COLOR_PICKING = 2,
    COLOR_DEPTH_PICKING = 3
};

enum ImageLayerType{
    COLOR_LAYER = 0,
    DEPTH_LAYER = 1,
    PICKING_LAYER = 2
};

static bool typeContainsColor(ImageType type){
    return (type == COLOR_ONLY || type == COLOR_DEPTH || type == COLOR_PICKING || type == COLOR_DEPTH_PICKING);
}

static bool typeContainsDepth(ImageType type){
    return (type == COLOR_DEPTH || type == COLOR_DEPTH_PICKING);
}

static bool typeContainsPicking(ImageType type){
    return (type == COLOR_PICKING || type == COLOR_DEPTH_PICKING);
}

} // namespace

#endif // IVW_IMAGETYPES_H
