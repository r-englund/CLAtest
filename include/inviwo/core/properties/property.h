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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_PROPERTY_H
#define IVW_PROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/properties/propertysemantics.h>
#include <inviwo/core/properties/propertyvisibility.h>
#include <inviwo/core/util/callback.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class Variant;

class IVW_CORE_API Property : public IvwSerializable, public VoidObservable, public VoidObserver {

public:
    Property(std::string identifier,
             std::string displayName,
             PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
             PropertySemantics semantics = PropertySemantics::Default);
    Property();
    virtual ~Property();

    virtual std::string getIdentifier() const;
    virtual void setIdentifier(const std::string& identifier);

    virtual std::string getClassName()  const { return "undefined"; }

    virtual std::string getDisplayName() const;
    virtual void setDisplayName(const std::string& displayName);

    virtual PropertySemantics getSemantics() const;
    virtual void setSemantics(const PropertySemantics& semantics);

    virtual void setReadOnly(const bool& value);
    virtual bool getReadOnly()const;

    PropertyOwner::InvalidationLevel getInvalidationLevel() const;
    void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) ;

    PropertyOwner* getOwner();
    virtual void setOwner(PropertyOwner* owner);

    void registerWidget(PropertyWidget* propertyWidget);
    void deregisterWidget(PropertyWidget* propertyWidget);
    /**
     *  This function should be called by propertywidgets before they initiate a property
     *  change. This is needed becouse when the property is modified it needs to update all
     *  of its widgets. And since it won't know if the change started in one of them we will
     *  update the propertywidget that started the change...
     *
     *  @param PropertyWidget* <#PropertyWidget* description#>
     */
    void setInitiatingWidget(PropertyWidget*);
    void clearInitiatingWidget();
    void updateWidgets();
    bool hasWidgets()const;
    MetaData* getMetaData(std::string meta);

    std::string getGroupID()const;
    void setGroupID(const std::string& groupID);

    std::string getGroupDisplayName()const;
    static void setGroupDisplayName(const std::string& groupID,const std::string& groupDisplayName);

    virtual void propertyModified();
    virtual void setPropertyModified(bool modified);
    virtual bool isPropertyModified() const;
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void set(const Property* src) { IVW_UNUSED_PARAM(src); }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    bool operator== (const Property& prop);

    template <typename T>
    void onChange(T* o, void (T::*m)()) {
        onChangeCallback_.addMemberFunction(o,m);
    }

    void setVisibility(PropertyVisibilityMode visibilityMode);
    PropertyVisibilityMode getVisibilityMode() {return visibilityMode_;};

    virtual void setVisible(bool val);
    virtual void updateVisibility();

protected:
    SingleCallBack onChangeCallback_;

private:
    std::string identifier_;
    std::string displayName_;

    bool readOnly_;
    PropertySemantics semantics_;
    PropertyVisibilityMode visibilityMode_;

    bool propertyModified_;
    PropertyOwner::InvalidationLevel invalidationLevel_;

    PropertyOwner* owner_;
    std::vector<PropertyWidget*> propertyWidgets_;

    std::string groupID_;
    std::string groupDisplayName_;
    static std::map<std::string,std::string> groupDisplayNames_;

    PropertyWidget* initiatingWidget_;

    //TODO: Use map (Refer Processor meta-data list)
    std::vector<MetaData*> metaData_;
};

} // namespace

#endif // IVW_PROPERTY_H
