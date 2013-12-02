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
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#ifndef IWE_ACTION_H
#define IWE_ACTION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

class IVW_CORE_API Action : public IvwSerializable {
public:
    Action();        
    virtual ~Action();

    std::string name() const { return actionName_; }
    int action() const { return action_; }

	virtual std::string getClassName() const { return "Undefined"; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    std::string actionName_;
    int action_;
};

} // namespace

#endif // IWE_ACTION_H