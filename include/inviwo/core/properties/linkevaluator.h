#ifndef IVW_LIKEVALUATOR_H
#define IVW_LIKEVALUATOR_H

#include "inviwo/core/properties/property.h"

namespace inviwo {

//TODO:
//Make this base class to support more evaluator types
class LinkEvaluator {
public:
   LinkEvaluator();
   void evaluate(Property* src, Property *dst);   
private:
   bool canLink(Property* src, Property *dst);
   bool canConvert(Variant& src, Variant& dst);
};

} // namespace

#endif // IVW_LIKEVALUATOR_H