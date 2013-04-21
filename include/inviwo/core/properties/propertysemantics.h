#ifndef IVW_PROPERTYSEMANTCIS_H
#define IVW_PROPERTYSEMANTCIS_H

namespace inviwo {
   /** class PropertySemantics
    *   A class that holds enum values for the property semantics of properties.
    *   The property semantics determine wich property widget to use.
    *   
    * @See PropertyWidgetFactoryQt
    */
class IVW_CORE_API PropertySemantics {

public:
    enum Type { Default =0,
                Color =1,
                LightPosition =2,
                Editor=3};
};
}

#endif //IVW_PROPERTYSEMANTCIS_H