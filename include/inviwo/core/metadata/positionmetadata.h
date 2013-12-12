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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_POSITION_METADATA_H
#define IVW_POSITION_METADATA_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/metadata/metadata.h>


namespace inviwo {

class IVW_CORE_API PositionMetaData : public IVec2MetaData {

public:
    PositionMetaData();
    PositionMetaData(int x, int y);
    virtual ~PositionMetaData();

    virtual std::string getClassName() const { return "PositionMetaData"; }
    virtual PositionMetaData* clone() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    ivec2 getXY();
    void setXY(const ivec2 &xy);

    int getX();
    void setX(const int &x);

    int getY();
    void setY(const int &y);

};

} // namespace

#endif // IVW_PROCESSOR_H
