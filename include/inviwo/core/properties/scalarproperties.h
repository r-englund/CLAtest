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
 * Main file authors: Peter Steneteg, Timo Ropinski, Sathish Kottravel
 *
 *********************************************************************************/

#ifndef IVW_SCALARPROPERTIES_H
#define IVW_SCALARPROPERTIES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/ordinalproperty.h>

namespace inviwo {

class IVW_CORE_API FloatProperty : public OrdinalProperty<float> {
public:
    FloatProperty(std::string identifier, std::string displayName, float value = 0.f,
                  float minValue=0.0f, float maxValue=1.0f, float increment=0.01f,
                  PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                  PropertySemantics semantics=PropertySemantics::Default);
    virtual std::string getClassName()  const { return "FloatProperty"; }
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

class IVW_CORE_API DoubleProperty : public OrdinalProperty<double> {
public:
    DoubleProperty(std::string identifier, std::string displayName, double value = 0.0,
                   double minValue = 0.0f, double maxValue = 1.0f, double increment = 0.01f,
                   PropertyOwner::InvalidationLevel invalidationLevel = PropertyOwner::INVALID_OUTPUT,
                   PropertySemantics semantics = PropertySemantics::Default);
    virtual std::string getClassName()  const { return "DoubleProperty"; }
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

class IVW_CORE_API IntProperty : public OrdinalProperty<int> {
public:
    IntProperty(std::string identifier, std::string displayName, int value = 0,
                int minValue=0, int maxValue=4096, int increment=1,
                PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                PropertySemantics semantics=PropertySemantics::Default);
    virtual std::string getClassName()  const { return "IntProperty"; }
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_SCALARPROPERTIES_H