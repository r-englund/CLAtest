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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/core/properties/propertysemantics.h>

namespace inviwo {

std::string getPropertySemanticID(PropertySemantics::Type type) {
    switch (type) {
        case PropertySemantics::Default:        return "Default";
        case PropertySemantics::Color:          return "Color";
        case PropertySemantics::LightPosition:  return "LightPosition";
        case PropertySemantics::Editor:         return "Editor";
        case PropertySemantics::Shader:         return "Shader";
        default: return "Unknown PropertySemantics.";
    }
}

} // namespace