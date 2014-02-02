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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_PROCESSORSTATE_H
#define IVW_PROCESSORSTATE_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

IVW_CORE_API enum CodeState {
    CODE_STATE_BROKEN,
    CODE_STATE_EXPERIMENTAL,
    CODE_STATE_STABLE
};

} // namespace

#endif // IVW_PROCESSORSTATE_H
