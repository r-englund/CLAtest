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

#ifndef IVW_PROPERTYWIDGET_H
#define IVW_PROPERTYWIDGET_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/metadata/propertyeditorwidgetmetadata.h>

namespace inviwo {

class Property;
class PropertyEditorWidget;

class IVW_CORE_API PropertyWidget {

public:
    PropertyWidget();
    PropertyWidget(Property* property);

    virtual ~PropertyWidget();

    virtual void updateFromProperty() = 0;
    virtual void showWidget() = 0;
    virtual void hideWidget() = 0;

    void setEditorWidget(PropertyEditorWidget* processorWidget);
    PropertyEditorWidget* getEditorWidget() const;
    bool hasEditorWidget() const;

    virtual Property* getProperty();
    virtual void setProperty(Property* property);
protected:
    Property* property_; //< Non owning reference to a property.
    PropertyEditorWidget* propertyEditor_;  //Additional widgets owned by property widget.
};


//Additional widget owned by property widget

class IVW_CORE_API PropertyEditorWidget {

public:
    PropertyEditorWidget();
    virtual ~PropertyEditorWidget();
    virtual void initialize(Property* property);
    virtual void setEditorVisibility(bool visible);
    virtual void showEditor();
    virtual void hideEditor();
    virtual void setEditorDimension(ivec2 dimension);
    virtual void moveEditor(ivec2 pos);
    virtual bool getEditorVisibilityMetaData();
    virtual ivec2 getEditorPositionMetaData();
    virtual ivec2 getEditorDimensionMetaData();

protected:
    PropertyEditorWidgetMetaData* metaData_;  //< Non owning reference to a metadata that belongs to property.
};

} // namespace

#endif // IVW_PROPERTYWIDGET_H
