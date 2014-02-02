/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Link�ping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sund�n
 *
 **********************************************************************/

#ifndef IVW_REPRESENTATIONCONVERTER_H
#define IVW_REPRESENTATIONCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <vector>

namespace inviwo {

class DataRepresentation;

class IVW_CORE_API RepresentationConverter {

public:
    RepresentationConverter();
    virtual ~RepresentationConverter();

    /**
     * Checks if it is possible to convert from the data representation.
     * @param source is the DataRepresentation to test for conversion possibility.
     * @return boolean True if possible, false otherwise.
     */
    virtual bool canConvertFrom(const DataRepresentation* source) const = 0;
    virtual bool canConvertTo(const DataRepresentation* destination) const = 0;

    virtual DataRepresentation* createFrom(const DataRepresentation* source) = 0;
    virtual void update(const DataRepresentation* source, DataRepresentation* destination) = 0;
};

template <typename TO>
class RepresentationConverterType : public RepresentationConverter {
public:
    virtual ~RepresentationConverterType() {};

    bool canConvertTo(const DataRepresentation* destination) const {
        return dynamic_cast<const TO*>(destination) != NULL;
    }
};


template <typename T>
class RepresentationConverterPackage : public RepresentationConverter {
public:
    RepresentationConverterPackage() : RepresentationConverter() {
        converters_ = new std::vector<RepresentationConverter*>();
    };
    ~RepresentationConverterPackage() {
        for (std::vector<RepresentationConverter*>::iterator it = converters_->begin() ; it != converters_->end(); ++it)
            delete(*it);

        delete converters_;
    }
    bool canConvertFrom(const DataRepresentation* source) const {
        for (std::vector<RepresentationConverter*>::const_iterator it = converters_->begin() ; it != converters_->end(); ++it) {
            if ((*it)->canConvertFrom(source))
                return true;
        }

        return false;
    }
    bool canConvertTo(const DataRepresentation* destination) const {
        for (std::vector<RepresentationConverter*>::const_iterator it = converters_->begin() ; it != converters_->end(); ++it) {
            if ((*it)->canConvertTo(destination))
                return true;
        }

        return false;
    }
    DataRepresentation* createFrom(const DataRepresentation* source) {
        for (std::vector<RepresentationConverter*>::iterator it = converters_->begin() ; it != converters_->end(); ++it) {
            if ((*it)->canConvertFrom(source))
                return (*it)->createFrom(source);
        }

        return NULL;
    }
    virtual void update(const DataRepresentation* source, DataRepresentation* destination) {
        for (std::vector<RepresentationConverter*>::iterator it = converters_->begin() ; it != converters_->end(); ++it) {
            if ((*it)->canConvertFrom(source))
                (*it)->update(source, destination);
        }
    }

    void addConverter(RepresentationConverter* converter) { converters_->push_back(converter); }
    size_t getNumberOfConverters() { return converters_->size(); }

    const std::vector<RepresentationConverter*>* getConverters() const { return converters_; }
private:
    std::vector<RepresentationConverter*>* converters_;
};

} // namespace

#endif // IVW_REPRESENTATIONCONVERTER_H
