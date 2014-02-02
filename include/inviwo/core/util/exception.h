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

#ifndef IVW_EXCEPTION_H
#define IVW_EXCEPTION_H

#include <iostream>

namespace inviwo {

class Exception : public std::exception {

public:
    Exception(const std::string& message = "");
    virtual ~Exception() throw() {}

    virtual const std::string getMessage() const throw();

protected:
    std::string message_;
};


class IgnoreException : public Exception {
public:
    IgnoreException(const std::string& message = "");
    virtual ~IgnoreException() throw() {}
};

class AbortException : public Exception {
public:
    AbortException(const std::string& message = "");
    virtual ~AbortException() throw() {}
};

class SerializationException : public Exception {
public:
    SerializationException(const std::string& message = "");
    virtual ~SerializationException() throw() {}
};

} // namespace

#endif // IVW_EXCEPTION_H
