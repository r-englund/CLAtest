#ifndef IVW_SERIALIZE_BASE_H
#define IVW_SERIALIZE_BASE_H

#include "../ext/ticpp/ticpp.h"

#include "inviwo/core/inviwofactorybase.h"


namespace inviwo {

class IvwSerializeBase{
    virtual ~IvwSerializeBase();
    virtual void registerFactories() const=0;
    virtual void registerFactory()=0;
};

} //namespace
#endif