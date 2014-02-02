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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

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
    void updateWidgets();
    bool hasWidgets()const;
    MetaData* getMetaData(std::string meta);

    void setGroupID(const std::string& groupID);

    static void setGroupDisplayName(const std::string& groupID,const std::string& groupDisplayName);

    std::string getGroupID()const;
    std::string getGroupDisplayName()const;

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

    //TODO: Use map (Refer Processor meta-data list)
    std::vector<MetaData*> metaData_;
};

} // namespace

#endif // IVW_PROPERTY_H
