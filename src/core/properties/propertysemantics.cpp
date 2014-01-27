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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include <inviwo/core/properties/propertysemantics.h>

namespace inviwo {

PropertySemantics::PropertySemantics() : semantic_("Default") {}
PropertySemantics::PropertySemantics(std::string semantic) : semantic_(semantic) {}
PropertySemantics::PropertySemantics(const PropertySemantics& rhs) : semantic_(rhs.semantic_) {};
PropertySemantics& PropertySemantics::operator=(const PropertySemantics& that){
    if (this != &that) {
        semantic_ = that.semantic_;
    }
    return *this;
}

const std::string& PropertySemantics::getString() const{
    return semantic_;
}

std::ostream& operator << (std::ostream& os, const inviwo::PropertySemantics& obj) {
    os << obj.semantic_;
    return os;
}

const PropertySemantics PropertySemantics::Default("Default");
const PropertySemantics PropertySemantics::Color("Color");
const PropertySemantics PropertySemantics::LightPosition("LightPosition");
const PropertySemantics PropertySemantics::Editor("Editor");
const PropertySemantics PropertySemantics::TextEditor("TextEditor");
const PropertySemantics PropertySemantics::ImageEditor("ImageEditor");
const PropertySemantics PropertySemantics::Shader("Shader");
    
} // namespace