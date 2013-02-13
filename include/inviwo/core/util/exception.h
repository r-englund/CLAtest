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

} // namespace

#endif // IVW_EXCEPTION_H
