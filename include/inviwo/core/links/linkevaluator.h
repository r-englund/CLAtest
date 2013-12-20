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

#ifndef IVW_LIKEVALUATOR_H
#define IVW_LIKEVALUATOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

//TODO:
//Make this base class to support more evaluator types
class IVW_CORE_API LinkEvaluator {
public:
   LinkEvaluator();
   void evaluate(Property* src, Property *dst);   
private:
   bool canLink(Property* src, Property *dst);
   static bool canConvert(const Variant& src, const Variant& dst);   
};

} // namespace

#endif // IVW_LIKEVALUATOR_H