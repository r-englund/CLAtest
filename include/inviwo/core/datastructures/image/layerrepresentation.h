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

#ifndef IVW_LAYERREPRESENTATION_H
#define IVW_LAYERREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/datastructures/image/layer.h>

namespace inviwo {

class IVW_CORE_API LayerRepresentation : public DataRepresentation {

friend class Layer;

public:
    LayerRepresentation(uvec2 dimensions, const DataFormatBase* format);
    virtual ~LayerRepresentation();
    virtual void performOperation(DataOperation*) const {};
    virtual void resize(uvec2 dimensions);
    const uvec2& getDimensions() const {return dimensions_;}
    // Removes old data and reallocate for new dimension.
    // Needs to be overloaded by child classes.
    virtual void setDimensions(uvec2 dimensions) { dimensions_ = dimensions;}
    virtual bool copyAndResizeLayer(DataRepresentation*) = 0;
    virtual DataRepresentation* clone() const = 0;
    virtual std::string getClassName() const { return "LayerRepresentation"; }

protected:
    uvec2 dimensions_;
};

} // namespace

#endif // IVW_LAYERREPRESENTATION_H
