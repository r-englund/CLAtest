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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/core/util/exception.h>

namespace inviwo {

Exception::Exception(const std::string& message)
    : message_(message) {}

const std::string Exception::getMessage() const throw() {
    return message_;
};

IgnoreException::IgnoreException(const std::string& message)
    : Exception(message) {}

AbortException::AbortException(const std::string& message)
    : Exception(message) {}

SerializationException::SerializationException(const std::string& message)
    : Exception(message) {}

} // namespace
