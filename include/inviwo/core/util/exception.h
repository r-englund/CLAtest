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
