#ifndef IVW_LIKEVALUATOR_H
#define IVW_LIKEVALUATOR_H

#include <inviwo/core/inviwocoredefine.h>
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