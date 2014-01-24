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

#ifndef IVW_PROCESSOR_WIDGET_METADATA_H
#define IVW_PROCESSOR_WIDGET_METADATA_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/metadata/positionmetadata.h>


namespace inviwo {

class IVW_CORE_API ProcessorWidgetMetaData : public MetaData {

public:
    ProcessorWidgetMetaData();
    ProcessorWidgetMetaData(const ProcessorWidgetMetaData& rhs);
    ProcessorWidgetMetaData& operator=(const ProcessorWidgetMetaData& that);
    virtual ~ProcessorWidgetMetaData();

    virtual std::string getClassName() const { return "ProcessorWidgetMetaData"; }
    virtual ProcessorWidgetMetaData* clone() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    void setWidgetPosition(ivec2 pos);
    ivec2 getWidgetPosition();
    void setDimension(ivec2 dim); 
    ivec2 getDimension() const;
    void setVisibile(bool visibility);
    bool isVisible() const;

private:
    IVec2MetaData positionMetaData_;
    IVec2MetaData dimensionMetaData_;
    BoolMetaData visiblityMetaData_;

};

} // namespace

#endif // IVW_PROCESSOR_WIDGET_METADATA_H
