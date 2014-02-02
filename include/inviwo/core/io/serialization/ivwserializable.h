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

#ifndef IVW_SERIALIZABLE_H
#define IVW_SERIALIZABLE_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

class IvwSerializer;
class IvwDeserializer;
class IvwSerializeBase;

class IVW_CORE_API IvwSerializable {
public:
    virtual ~IvwSerializable() {}
    virtual void serialize(IvwSerializer& s) const=0;
    virtual void deserialize(IvwDeserializer& d)=0;
};

} //namespace
#endif