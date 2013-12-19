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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#ifndef IVW_PROPERTYSEMANTICS_H
#define IVW_PROPERTYSEMANTICS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <string>

namespace inviwo {

class IVW_CORE_API PropertySemantics {
public:
    enum Type { Default=0,
                Color=1,
                LightPosition=2,
                Editor=3,
				Shader=4};
};

IVW_CORE_API std::string getPropertySemanticID(PropertySemantics::Type type);

} //Namespace

#endif //IVW_PROPERTYSEMANTICS_H