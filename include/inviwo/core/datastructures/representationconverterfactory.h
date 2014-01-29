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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_REPRESENTATIONCONVERTERFACTORY_H
#define IVW_REPRESENTATIONCONVERTERFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API RepresentationConverterFactory : public Singleton<RepresentationConverterFactory>  {

public:
    RepresentationConverterFactory();
    virtual ~RepresentationConverterFactory();

    void registerObject(RepresentationConverter* representationConverter);
    
    template <typename T>
    RepresentationConverter* getRepresentationConverter(DataRepresentation* source) {
        // TODO: optimize performance, e.g., by using a hash table
        for (size_t i=0; i<representationConverters_.size(); i++) {
            RepresentationConverterType<T>* repConverterTyped = dynamic_cast<RepresentationConverterType<T>*>(representationConverters_[i]);
            if (repConverterTyped){
                if (repConverterTyped->canConvertFrom(source)){
                    return representationConverters_[i];
                }
            }
        }
        return NULL;
    }

    template <typename T>
    RepresentationConverterPackage<T>* getRepresentationConverterPackage(DataRepresentation* source) {
        // TODO: optimize performance, e.g., by using a hash table
        RepresentationConverterPackage<T>* currentConverterPackage = NULL; 
        for (size_t i=0; i<representationConverters_.size(); i++) {
            RepresentationConverterPackage<T>* repConverterPackage = dynamic_cast<RepresentationConverterPackage<T>*>(representationConverters_[i]);
            if (repConverterPackage){
                if (repConverterPackage->canConvertFrom(source)){
                    if (currentConverterPackage)
                        currentConverterPackage = (repConverterPackage->getNumberOfConverters() < currentConverterPackage->getNumberOfConverters() ? repConverterPackage : currentConverterPackage);
                    else
                        currentConverterPackage = repConverterPackage;
                }
            }
        }
        return currentConverterPackage;
    }

private:
    std::vector<RepresentationConverter*> representationConverters_;
};

} // namespace

#endif // IVW_REPRESENTATIONCONVERTERFACTORY_H
