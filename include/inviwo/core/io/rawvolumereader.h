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

#ifndef IVW_RAWVOLUMEREADER_H
#define IVW_RAWVOLUMEREADER_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/volumereader.h"

namespace inviwo {

class IVW_CORE_API RawVolumeReader : public VolumeReader {
public:
	RawVolumeReader();
	virtual ~RawVolumeReader() {}

protected:

};

} // namespace

#endif // IVW_RAWVOLUMEREADER_H
