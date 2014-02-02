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

#ifndef IVW_FORMATCONVERSION_H
#define IVW_FORMATCONVERSION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <string>
#include <sstream>

namespace inviwo {

#define BYTE_SWAP 1024

static const uint64_t BYTE_SIZE = 1;
static const uint64_t KILO_BYTE_SIZE = BYTE_SWAP* BYTE_SIZE;
static const uint64_t MEGA_BYTE_SIZE = BYTE_SWAP* KILO_BYTE_SIZE;
static const uint64_t GIGA_BYTE_SIZE = BYTE_SWAP* MEGA_BYTE_SIZE;
static const uint64_t TERA_BYTE_SIZE = BYTE_SWAP* GIGA_BYTE_SIZE;
static const float BYTE_DIV = 1.f/BYTE_SWAP;

#define BYTES_TO_KILOBYTES(bytes) (bytes/BYTE_SWAP)
#define BYTES_TO_MEGABYTES(bytes) (bytes/(BYTE_SWAP*BYTE_SWAP))
#define KILOBYTES_TO_BYTES(bytes) (bytes*BYTE_SWAP)
#define MEGABYTES_TO_BYTES(bytes) (bytes*BYTE_SWAP*BYTE_SWAP)

STARTCLANGIGNORE("-Wunused-function")
static std::string formatBytesToString(uint64_t bytes) {
    std::ostringstream stream;
    stream.precision(2);
    stream.setf(std::ios::fixed, std::ios::floatfield);

    if (bytes > TERA_BYTE_SIZE)
        stream << static_cast<float>(bytes/GIGA_BYTE_SIZE)*BYTE_DIV << " TB";
    else if (bytes > GIGA_BYTE_SIZE)
        stream << static_cast<float>(bytes/MEGA_BYTE_SIZE)*BYTE_DIV << " GB";
    else if (bytes > MEGA_BYTE_SIZE)
        stream << static_cast<float>(bytes/KILO_BYTE_SIZE)*BYTE_DIV << " MB";
    else if (bytes > KILO_BYTE_SIZE)
        stream << static_cast<float>(bytes)*BYTE_DIV << " KB";
    else
        stream << static_cast<float>(bytes) << " B";

    return stream.str();
};
ENDCLANGIGNORE

} // namespace

#endif // IVW_FORMATCONVERSION_H
