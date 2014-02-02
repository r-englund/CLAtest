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

#include <inviwo/core/datastructures/volume/volumeramconverter.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

VolumeDisk2RAMConverter::VolumeDisk2RAMConverter()
    : RepresentationConverterType<VolumeRAM>()
{}

VolumeDisk2RAMConverter::~VolumeDisk2RAMConverter() {}

DataRepresentation* VolumeDisk2RAMConverter::createFrom(const DataRepresentation* source) {
    const VolumeDisk* volumeDisk = static_cast<const VolumeDisk*>(source);

    if (volumeDisk) {
        switch (volumeDisk->getDataFormatId()) {
#define DataFormatIdMacro(i) case i: return new VolumeRAM_##i(static_cast<Data##i::type*>(const_cast<VolumeDisk*>(volumeDisk)->readData()), volumeDisk->getDimension());
#include <inviwo/core/util/formatsdefinefunc.h>

            default:
                LogError("Cannot convert format from Disk to RAM:" << volumeDisk->getDataFormat()->getString());
        }
    }

    return NULL;
}
void VolumeDisk2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeDisk* volumeSrc = static_cast<const VolumeDisk*>(source);
    VolumeRAM* volumeDst = static_cast<VolumeRAM*>(destination);

    if (volumeSrc->getDimension() != volumeDst->getDimension())
        volumeDst->setDimension(volumeSrc->getDimension());

    volumeSrc->readDataInto(volumeDst->getData());
}

} // namespace
