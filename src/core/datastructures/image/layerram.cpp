/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

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
        case DataFormatEnums::NOT_SPECIALIZED:
            LogErrorCustom("createLayerRAM", "Invalid format");
            return NULL;
#define DataFormatIdMacro(i) case DataFormatEnums::i: return new LayerRAMCustomPrecision<Data##i::type, Data##i::bits>(dimension, type); break;
#include <inviwo/core/util/formatsdefinefunc.h>

        default:
            LogErrorCustom("createLayerRAM", "Invalid format or not implemented");
            return NULL;
    }

    return NULL;
}

} // namespace
