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
    virtual void initializeEditorWidgetsMetaData()=0;
    Property* property_; //< Non owning reference to a property.
    PropertyEditorWidget* propertyEditor_;  //< Reference to additional widgets (not necessarily owned always)
};


//Additional widget owned by property widget

class IVW_CORE_API PropertyEditorWidget {

public:
    PropertyEditorWidget();
    virtual ~PropertyEditorWidget();
    virtual void initialize(Property* property)=0;
    virtual void deinitialize()=0;
    //set functions
    virtual void setEditorVisibility(bool visible);
    virtual void showEditor();
    virtual void hideEditor();
    virtual void setEditorDimension(ivec2 dimension);
    virtual void moveEditor(ivec2 pos);
    virtual void setDockStatus(PropertyEditorWidgetDockStatus dockStatus);
    //get functions
    virtual bool getEditorVisibilityMetaData();
    virtual ivec2 getEditorPositionMetaData();
    virtual ivec2 getEditorDimensionMetaData();
    virtual PropertyEditorWidgetDockStatus getEditorDockStatus();

protected:
    PropertyEditorWidgetMetaData* metaData_;  //< Non owning reference to a metadata that belongs to property.
};

} // namespace

#endif // IVW_PROPERTYWIDGET_H
