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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_IMAGETYPES_H
#define IVW_IMAGETYPES_H

namespace inviwo {

typedef enum ImageType{
    COLOR_ONLY = 0,
    COLOR_DEPTH = 1,
    COLOR_PICKING = 2,
    COLOR_DEPTH_PICKING = 3
} ImageType;

typedef enum LayerType{
    COLOR_LAYER = 0,
    DEPTH_LAYER = 1,
    PICKING_LAYER = 2
} LayerType;

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
