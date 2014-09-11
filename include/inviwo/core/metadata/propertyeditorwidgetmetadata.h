/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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

#ifndef IVW_PROPERTY_EDITOR_WIDGET_METADATA_H
#define IVW_PROPERTY_EDITOR_WIDGET_METADATA_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/metadata/positionmetadata.h>


namespace inviwo {

class IVW_CORE_API PropertyEditorWidgetDockStatus {
public:
    PropertyEditorWidgetDockStatus();
    PropertyEditorWidgetDockStatus(std::string dockStatus);
    PropertyEditorWidgetDockStatus(const PropertyEditorWidgetDockStatus& rhs);
    virtual ~PropertyEditorWidgetDockStatus() {}
    PropertyEditorWidgetDockStatus& operator=(const PropertyEditorWidgetDockStatus& rhs);
    bool operator==(const PropertyEditorWidgetDockStatus& that);
    const std::string& getString() const;
    static const PropertyEditorWidgetDockStatus Floating;
    static const PropertyEditorWidgetDockStatus DockedLeft;
    static const PropertyEditorWidgetDockStatus DockedRight;
private:
    std::string dockStatus_;
};

//////////////////////////////////////////////////////////////////////////

class IVW_CORE_API PropertyEditorWidgetMetaData : public MetaData {

public:

    PropertyEditorWidgetMetaData();
    PropertyEditorWidgetMetaData(const PropertyEditorWidgetMetaData& rhs);
    PropertyEditorWidgetMetaData& operator=(const PropertyEditorWidgetMetaData& that);
    virtual ~PropertyEditorWidgetMetaData();

    virtual std::string getClassIdentifier() const { return "PropertyEditorWidgetMetaData"; }
    virtual PropertyEditorWidgetMetaData* clone() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual bool equal(const MetaData& rhs) const;

    void setWidgetPosition(ivec2 pos);
    ivec2 getWidgetPosition();
    void setDimension(ivec2 dim);
    ivec2 getDimension() const;
    void setVisibile(bool visibility);
    bool isVisible() const;
    void setDockStatus(PropertyEditorWidgetDockStatus& dockStatus);
    const PropertyEditorWidgetDockStatus getDocStatus() const;

private:
    IVec2MetaData positionMetaData_;
    IVec2MetaData dimensionMetaData_;
    BoolMetaData visiblityMetaData_;
    StringMetaData dockStatusMetaData_;
};

} // namespace

#endif // IVW_PROPERTY_EDITOR_WIDGET_METADATA_H
