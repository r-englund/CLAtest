#ifndef IVW_PROJECT_H
#define IVW_PROJECT_H

#include <fstream>
#include "inviwo/core/inviwo.h"
#include "ext/voreen/serialization/networkserializer.h"

namespace inviwo {

class Project : public Serializable {

public:
    Project();
    virtual ~Project();

    void load(std::string projectName) throw (Exception);
    
    ProcessorNetwork* getProcessorNetwork();

    virtual void serialize(XmlSerializer& s) const;
    virtual void deserialize(XmlDeserializer& s);

private:
    ProcessorNetwork* processorNetwork_;

};

} // namespace

#endif // IVW_PROJECT_H
