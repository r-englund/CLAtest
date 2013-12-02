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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_VOLUMERAMCONVERTER_H
#define IVW_VOLUMERAMCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>

namespace inviwo {

    class IVW_CORE_API VolumeDisk2RAMConverter : public RepresentationConverterType<VolumeRAM> {

    public:
        VolumeDisk2RAMConverter();
        virtual ~VolumeDisk2RAMConverter();

        inline bool canConvertFrom(const DataRepresentation* source) const {
            return dynamic_cast<const VolumeDisk*>(source) != NULL;
        }

        DataRepresentation* createFrom(const DataRepresentation* source);
        void update(const DataRepresentation* source, DataRepresentation* destination);
    };

} // namespace

#endif // IVW_VOLUMERAMCONVERTER_H
