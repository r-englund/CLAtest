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

#ifndef IVW_COMPOSITE_METADATA_H
#define IVW_COMPOSITE_METADATA_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/metadata/metadata.h>

namespace inviwo {

class IVW_CORE_API CompositeMetaData : public MetaData {

protected:
    CompositeMetaData();
    virtual ~CompositeMetaData();

    virtual std::string getClassName() const { return "CompositeMetaData"; }
    virtual CompositeMetaData* create() const;
    void addMetaData(MetaData* metaData);
    void addMetaData(MetaData& metaData);
    std::vector<MetaData*> getSubMetaData() { return subMetaData_; }
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

private:
    std::vector<MetaData*> subMetaData_;
    
};

} // namespace

#endif // IVW_COMPOSITE_METADATA_H