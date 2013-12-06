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

#ifndef IVW_MeshCreator_H
#define IVW_MeshCreator_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/datastructures/geometry/simplemesh.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/scalarproperties.h>

namespace inviwo {

class IVW_MODULE_BASE_API MeshCreator : public Processor {
public:
    MeshCreator();
    ~MeshCreator();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    SimpleMesh* createMesh();

protected:
    virtual void process();

private:
    GeometryOutport outport_;
    FloatProperty meshScale_; // Scale size of mesh

    OptionPropertyString meshType_;
};

} // namespace

#endif // IVW_MeshCreator_H
