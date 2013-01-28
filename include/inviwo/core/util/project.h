#ifndef IVW_PROJECT_H
#define IVW_PROJECT_H

#include <inviwo/core/inviwocoredefine.h>
#include <fstream>
#include <inviwo/core/inviwo.h>
//#include <voreen/serialization/networkserializer.h>
#include <inviwo/core/network/processornetwork.h>

namespace inviwo {

class IVW_CORE_API Project : public IvwSerializable {

public:
    Project();
    virtual ~Project();

    void load(std::string projectName) throw (Exception);
    
    ProcessorNetwork* getProcessorNetwork();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

private:
    ProcessorNetwork* processorNetwork_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_PROJECT_H
