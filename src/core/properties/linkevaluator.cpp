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

#include <inviwo/core/properties/linkevaluator.h>
#include <inviwo/core/util/variant.h>

namespace inviwo {

LinkEvaluator::LinkEvaluator() {}

void LinkEvaluator::evaluate(Property* src, Property *dst) {
    ivwAssert(src!=0, "source property expected");
    ivwAssert(dst!=0, "destination property expected");
    if (canLink(src, dst)) {
        dst->set(src);
    }
}

bool LinkEvaluator::canLink(Property* src, Property *dst) {
    if (*src == *dst) return true;
    return canConvert(src->getVariant(), dst->getVariant());
}

bool LinkEvaluator::canConvert(const Variant& src, const Variant& dst) {
    if (src.getType() == Variant::VariantTypeInvalid || dst.getType() == Variant::VariantTypeInvalid) {
        //Error message
        return false;
    }
    return Variant::canConvert(src.getType(), dst.getType());
}

}