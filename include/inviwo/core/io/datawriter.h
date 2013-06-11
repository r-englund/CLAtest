#ifndef IVW_DATAWRITER_H
#define IVW_DATAWRITER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include "inviwo/core/datastructures/data.h"

namespace inviwo {

class IVW_CORE_API DataWriter {

public:
    DataWriter();
    virtual ~DataWriter();

    std::string getIdentifier() const;
    virtual void writeData()=0;

protected:

    void setIdentifier(const std::string& name);

private:
    std::string identifier_;
};

} // namespace

#endif // IVW_DATAWRITER_H
