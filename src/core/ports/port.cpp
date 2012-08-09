#include "inviwo/core/ports/port.h"

namespace inviwo {

    Port::Port(PortDirection direction, std::string identifier)
        : direction_(direction),
          identifier_(identifier)
    {}

    Port::~Port() {}

    void Port::initialize() {}
    void Port::deinitialize() {}

    void Port::connectTo(Port* /*port*/) {
        connected_ = true;
    }

} // namespace
