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

#ifndef IVW_IMAGESOURCESERIES_H
#define IVW_IMAGESOURCESERIES_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/scalarproperties.h>

namespace inviwo {

class IVW_MODULE_BASE_API ImageSourceSeries : public Processor {
public:
    ImageSourceSeries();
    ~ImageSourceSeries();

    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();
    virtual void onFindFiles();

protected:
    virtual void process();

private:
    ImageOutport outport_;
    ButtonProperty findFilesButton_;
    DirectoryProperty imageFileDirectory_;
    IntProperty currentImageIndex_;
};

} // namespace

#endif // IVW_IMAGESOURCESERIES_H
