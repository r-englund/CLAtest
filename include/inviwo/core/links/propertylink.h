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

#ifndef IVW_PROPERYLINK_H
#define IVW_PROPERYLINK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/links/linkevaluator.h>

namespace inviwo {

class IVW_CORE_API PropertyLink : public IvwSerializable {
public :
    PropertyLink();
    PropertyLink(Property* srcProperty, Property* dstProperty);
    virtual ~PropertyLink();

    void setSourceProperty(Property* src) { srcProperty_=src; }
    void setDestinationProperty(Property* dst) { dstProperty_=dst; }

    Property* getSourceProperty() const { return srcProperty_; }
    Property* getDestinationProperty() const { return dstProperty_; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    void switchDirection();

private:
    Property* srcProperty_;
    Property* dstProperty_;
};

} // namespace

#endif // IVW_PROPERYLINK_H
