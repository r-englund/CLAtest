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

#ifndef IVW_IMAGEREPRESENTATION_H
#define IVW_IMAGEREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/datagrouprepresentation.h>
#include <inviwo/core/datastructures/image/image.h>

namespace inviwo {

class Image;

class IVW_CORE_API ImageRepresentation : public DataGroupRepresentation {

    friend class Image;

public:
    ImageRepresentation();
    ImageRepresentation(const ImageRepresentation& rhs);
    ImageRepresentation& operator=(const ImageRepresentation& that);
    virtual ImageRepresentation* clone() const = 0;
    virtual ~ImageRepresentation();

    virtual void performOperation(DataOperation*) const;
    virtual std::string getClassName() const;

    uvec2 getDimension() const;

    virtual bool copyAndResizeRepresentation(DataRepresentation*) const = 0;

    const Image* getOwner() const;

protected:
    virtual void update(bool) = 0;

    virtual void setPointerToOwner(DataGroup*);

    Image* owner_;
};

} // namespace

#endif // IVW_IMAGEREPRESENTATION_H
