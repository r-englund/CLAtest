/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

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

    virtual std::string getClassName() const { return "PropertyEditorWidgetMetaData"; }
    virtual PropertyEditorWidgetMetaData* clone() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

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
