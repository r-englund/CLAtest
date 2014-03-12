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
#include <inviwo/core/properties/propertyobserver.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/properties/propertysemantics.h>
#include <inviwo/core/properties/propertyvisibility.h>
#include <inviwo/core/util/callback.h>


namespace inviwo {

class Variant;

/** \class Property
 * 
 *  Concepts:
 *   - Owner: A property can have a owner, usually a processor. If the property is modified, by 
 *     calling propertyModified() then the property will set it's owner's invalidation level to the 
 *     property's invalidation level, usually INVALID_OUTPUT. This will in turn trigger a network 
 *     evaluation that will update the processors to a valid state again.
 *   
 *   - Reset: A property has a default state specified in the constructor, or optionally be calling
 *     setCurrentStateAsDefault. The property can then also be reset to it's default state by calling
 *     resetToDefaultState. Both these functions are virtual and all property subclasses that
 *     introduce more state should make sure to implement these two functions and also in their
 *     implementation make sure that to call the base class implementation.
 *
 *   - Widget: A property can have one or multiple PropertyWidgets. The widget are used in the user 
 *     interface to implement interactivity.
 *     
 *   
 */


class IVW_CORE_API Property : public IvwSerializable, public PropertyObserver {

public:
    Property(std::string identifier,
             std::string displayName,
             PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
             PropertySemantics semantics = PropertySemantics::Default);
    Property();
    virtual ~Property();

    virtual std::string getClassName()  const { return "undefined"; }

    virtual std::string getIdentifier() const;
    virtual void setIdentifier(const std::string& identifier);
    virtual std::string getDisplayName() const;
    virtual void setDisplayName(const std::string& displayName);
    std::string getGroupID()const;
    void setGroupID(const std::string& groupID);
    std::string getGroupDisplayName()const;
    static void setGroupDisplayName(const std::string& groupID, const std::string& groupDisplayName);


    virtual PropertySemantics getSemantics() const;
    virtual void setSemantics(const PropertySemantics& semantics);

    virtual void setReadOnly(const bool& value);
    virtual bool getReadOnly()const;

    PropertyOwner::InvalidationLevel getInvalidationLevel() const;
    void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) ;

    PropertyOwner* getOwner();
    virtual void setOwner(PropertyOwner* owner);


    // Widget calls
    void registerWidget(PropertyWidget* propertyWidget);
    void deregisterWidget(PropertyWidget* propertyWidget);
    const std::vector<PropertyWidget*>& getWidgets() const { return propertyWidgets_; }
    
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

    /**
     *  Save the current state of the property as the default. This state will then be used as a 
     *  reference when serializing, only state different from the default will be serialized.
     *  This method should usually only be called once directly after construction in the processor 
     *  constructor after setting property specific state.
     *  It is important that all overriding properties make sure to call the base class implementation.
     */
    virtual void setCurrentStateAsDefault();
    
    /**
     * Reset the state of the property back to it's default value.
     * It is important that all overriding properties make sure to call the base class implementation.
     */
    virtual void resetToDefaultState();

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
    CallBackList onChangeCallback_;

private:
    std::string identifier_;
    std::string displayName_;
    bool readOnly_;
    bool defaultReadOnly_;

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
