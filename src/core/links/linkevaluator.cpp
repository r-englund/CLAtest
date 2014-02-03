 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/core/links/linkevaluator.h>
#include <inviwo/core/links/linkconditions.h>
#include <inviwo/core/util/variant.h>

namespace inviwo {

LinkEvaluator::LinkEvaluator() {}

void LinkEvaluator::evaluate(Property* src, Property* dst) {
    ivwAssert(src!=0, "source property expected");
    ivwAssert(dst!=0, "destination property expected");

    if (canLink(src, dst))
        dst->set(src);
}

bool LinkEvaluator::canLink(Property* src, Property* dst) {
    if (SimpleCondition::canLink(src,dst)) return true;

    return canConvert(src->getVariant(), dst->getVariant());
}

bool LinkEvaluator::canConvert(const Variant& src, const Variant& dst) {
    //Replace with variant based condition
    if (src.getType() == Variant::VariantTypeInvalid || dst.getType() == Variant::VariantTypeInvalid) {
        //Error message
        return false;
    }

    return Variant::canConvert(src.getType(), dst.getType());
}

}