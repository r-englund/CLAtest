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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_IMAGESOURCE_H
#define IVW_IMAGESOURCE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

class IVW_MODULE_BASE_API ImageSource : public Processor {
public:
    ImageSource();
    ~ImageSource();
    
    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();

protected:
    virtual void process();

private:
    ImageOutport outport_;
    FileProperty imageFileName_;
};

} // namespace

#endif // IVW_IMAGESOURCE_H
