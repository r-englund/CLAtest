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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/datastructures/representationconverterfactory.h>

namespace inviwo {

RepresentationConverterFactory::RepresentationConverterFactory() {}

RepresentationConverterFactory::~RepresentationConverterFactory() {}

void RepresentationConverterFactory::registerObject(RepresentationConverter* representationConverter) {
    if (std::find(representationConverters_.begin(),
                  representationConverters_.end(),
                  representationConverter) == representationConverters_.end())
        representationConverters_.push_back(representationConverter);
    else
        LogWarn("Could not add a RepresentationConverter becouse it was already added");
}

} // namespace
