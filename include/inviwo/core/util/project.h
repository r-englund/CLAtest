#ifndef IVW_PROJECT_H
#define IVW_PROJECT_H

#include <fstream>
#include "inviwo/core/inviwo.h"
//#include "ext/voreen/serialization/networkserializer.h"
#include "inviwo/core/network/processornetwork.h"

namespace inviwo {

class Project : public IvwSerializable {

public:
    Project();
    virtual ~Project();

    void load(std::string projectName) throw (Exception);
    
    ProcessorNetwork* getProcessorNetwork();

    virtual void serialize(IvwSerializeBase& s) const;
    virtual void deserialize(IvwSerializeBase& s);

private:
    ProcessorNetwork* processorNetwork_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_PROJECT_H
