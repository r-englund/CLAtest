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

#ifndef IVW_IMAGEDISK_H
#define IVW_IMAGEDISK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>

namespace inviwo {

class IVW_CORE_API ImageDisk : public ImageRepresentation {

public:
    ImageDisk();
    ImageDisk(const ImageDisk& rhs);
    ImageDisk& operator=(const ImageDisk& that);
    virtual ImageDisk* clone() const;
    virtual ~ImageDisk();

    virtual void initialize();
    virtual void deinitialize();

    virtual std::string getClassName() const;
    virtual bool copyAndResizeRepresentation(DataRepresentation*) const;

protected:
    virtual void update(bool editable);
};

} // namespace

#endif // IVW_IMAGEDISK_H
