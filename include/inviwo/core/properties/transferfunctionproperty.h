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
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#ifndef IVW_TRANSFERFUNCTIONPROPERTY_H
#define IVW_TRANSFERFUNCTIONPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/datastructures/transferfunction.h>

namespace inviwo {

class IVW_CORE_API TransferFunctionProperty : public TemplateProperty<TransferFunction> {

public:
    TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value = TransferFunction(),
                             PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                             PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "TransferFunctionProperty"; }

    IntMinMaxProperty* maskProperty();
    IntMinMaxProperty* zoomHorizontalProperty();
    FloatMinMaxProperty* zoomVerticalProperty();
    OptionPropertyInt* bitRangeProperty();

    void updateMask(ivec2 mask, int width);

private:
    IntMinMaxProperty maskProperty_;
    IntMinMaxProperty zoomHorizontalProperty_;
    FloatMinMaxProperty zoomVerticalProperty_;
    OptionPropertyInt bitRangeProperty_;
};

} // namespace inviwo

#endif // IVW_TRANSFERFUNCTIONPROPERTY_H