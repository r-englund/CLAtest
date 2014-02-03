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

#ifndef IVW_PROCESSORLINK_H
#define IVW_PROCESSORLINK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/links/linkevaluator.h>
#include <inviwo/core/links/propertylink.h>

namespace inviwo {

class IVW_CORE_API ProcessorLink : public IvwSerializable {

public:

    ProcessorLink();
    ProcessorLink(Processor* outProcessor, Processor* inProcessor);
    virtual ~ProcessorLink();

    Processor* getSourceProcessor() const { return sourceProcessor_; }
    Processor* getDestinationProcessor() const { return destinationProcessor_; }


    void autoLinkPropertiesByType();
    void evaluate(LinkEvaluator* leval);
    bool isLinked(Property* startProperty, Property* endProperty);
    bool involvesProcessor(Processor* processor);
    bool isPropertySource(Property* property) ;
    bool isPropertyDestination(Property* property) ;
    bool involvesProperty(Property* property) ;
    bool isValid();
    std::vector<Property*> getSourceProperties();
    std::vector<Property*> getDestinationProperties();
    void addPropertyLinks(Property* startProperty, Property* endProperty);
    void removePropertyLinks(Property* startProperty, Property* endProperty);
    void removePropertyLink(PropertyLink* plink);
    void removeBidirectionalPair(Property* startProperty, Property* endProperty);
    std::vector<PropertyLink*> getPropertyLinks() {return propertyLinks_;}
    PropertyLink* getPropertyLink(Property* startProperty, Property* endProperty);
    PropertyLink* getBidirectionalPair(PropertyLink* propertyLink);
    PropertyLink* getBidirectionalPair(Property* startProperty, Property* endProperty);
    std::string getLinkInfo();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

private:
    Processor* sourceProcessor_;
    Processor* destinationProcessor_;
    std::vector<PropertyLink*> propertyLinks_;
};

} // namespace

#endif // IVW_PROCESSORLINK_H
