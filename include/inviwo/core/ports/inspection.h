#ifndef IVW_INSPECTION_H
#define IVW_INSPECTION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/port.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/scalarproperties.h>

namespace inviwo {

class IVW_CORE_API Inspection {

public:
    Inspection();
    virtual ~Inspection();

    enum State {
        Start = 0,
        Wait = 1,
        Inspect = 2
    };

    bool isActive();
    int size();
    inviwo::Inspection::State state() const;
    void setState(inviwo::Inspection::State val);
    void setPort(Port* port);
    bool samePort(Port* port);
    void resetPort();

    std::string processorIdentifier_;
    std::string portIdentifier_;
    ivec2 pos_;

private:
    State state_;



    BoolProperty* active_;
    IntProperty* size_;
};


} // namespace

#endif // IVW_INSPECTION_H

