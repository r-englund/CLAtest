#ifndef IVW_INVIWO_H
#define IVW_INVIWO_H

#define IVW_DEBUG //TODO: move into project file

#pragma warning(disable: 4290)

#include <iostream>
#include <string>
#include <vector>

// error handling
#include "inviwo/core/util/assertion.h"
#include "inviwo/core/util/exception.h"
#define IVW_UNUSED_PARAM(param) (void)param

// include some old tgt and Voreen stuff
#include "ext/voreen/ext/tgt/quaternion.h"
#include "ext/voreen/ext/tgt/vector.h"
#include "ext/voreen/serialization/serialization.h"
using namespace tgt;
using namespace voreen;

#endif // IVW_INVIWO_H
