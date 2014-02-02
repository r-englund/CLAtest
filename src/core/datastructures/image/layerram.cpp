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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/image/layerramprecision.h>
#include <inviwo/core/io/imageio.h>

namespace inviwo {

LayerRAM::LayerRAM(uvec2 dimension, LayerType type, const DataFormatBase* format)
    : LayerRepresentation(dimension, type, format), data_(NULL) {
}

LayerRAM::LayerRAM(const LayerRAM& rhs)
    : LayerRepresentation(rhs) {
}

LayerRAM& LayerRAM::operator=(const LayerRAM& that) {
    if (this != &that)
        LayerRepresentation::operator=(that);

    return *this;
}

LayerRAM::~LayerRAM() {
}

std::string LayerRAM::getClassName() const {
    return "LayerRAM";
}

bool LayerRAM::copyAndResizeLayer(DataRepresentation* targetLayerRam) const {
    const LayerRAM* source = this;
    LayerRAM* target = dynamic_cast<LayerRAM*>(targetLayerRam);

    if (!target) {
        return false;
        LogError("Target representation missing.");
    }

    if (!source->getData())
        return true;

    if (source->getDimension() != target->getDimension()) {
        //CPU image rescaling using imageIO
        void* rawData = ImageIO::rescaleLayerRAM(source, target->getDimension());

        if (!rawData)
            return false;

        target->setData(rawData);
    }
    else {
        if (!target->getData()) {
            return false;
            LogError("Target should have data.");
        }

        memcpy(target->getData(), source->getData(), source->getDimension().x*source->getDimension().y*source->getDataFormat()->getBitsAllocated());
    }

    return true;
}

void LayerRAM::setDimension(uvec2 dimensions) {
    resize(dimensions);
}

void* LayerRAM::getData() {
    return data_;
}

const void* LayerRAM::getData() const {
    return data_;
}

void LayerRAM::setData(void* data) {
    deinitialize();
    data_ = data;
}

LayerRAM* createLayerRAM(const uvec2& dimension, LayerType type, const DataFormatBase* format) {
    switch (format->getId())
    {
        case NOT_SPECIALIZED:
            LogErrorCustom("createLayerRAM", "Invalid format");
            return NULL;
#define DataFormatIdMacro(i) case i: return new LayerRAMCustomPrecision<Data##i::type, Data##i::bits>(dimension, type); break;
#include <inviwo/core/util/formatsdefinefunc.h>

        default:
            LogErrorCustom("createLayerRAM", "Invalid format or not implemented");
            return NULL;
    }

    return NULL;
}

} // namespace
