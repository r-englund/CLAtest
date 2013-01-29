#ifndef IVW_DATAREADER_H
#define IVW_DATAREADER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include "inviwo/core/datastructures/data.h"

namespace inviwo {

class IVW_CORE_API DataReader {

public:
    DataReader();
    virtual ~DataReader();

    std::string getIdentifier() const;
    virtual Data* readData(const std::string filePath)=0;

protected:

    void setIdentifier(const std::string& name);

private:
    std::string identifier_;
};

} // namespace

#endif // IVW_DATAREADER_H
