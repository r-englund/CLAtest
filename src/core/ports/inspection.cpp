#include <inviwo/core/ports/inspection.h>

namespace inviwo {

Inspection::Inspection() : state_(State::Start)
    , processorIdentifier_("")
    , portIdentifier_("")
    , active_(NULL)
    , size_(NULL) {
}

Inspection::~Inspection() {
}

bool Inspection::isActive() {
    if(!active_) {
        if(InviwoApplication::getPtr()) {
            active_ = dynamic_cast<BoolProperty*>(
                InviwoApplication::getPtr()->
                getSettingsByType<SystemSettings>()->
                getPropertyByIdentifier("enablePortInspectors"));
        } else {
            return false;
        }
    }
    return active_->get();
}

int Inspection::size() {
    if(!size_) {
        if(InviwoApplication::getPtr()) {
            size_ = dynamic_cast<IntProperty*>(
                InviwoApplication::getPtr()->
                getSettingsByType<SystemSettings>()->
                getPropertyByIdentifier("portInspectorSize"));
        } else {
            return 128;
        }
    }
    return size_->get();
}

inviwo::Inspection::State Inspection::state() const {
    return state_;
}

void Inspection::setState(State val) {
    state_ = val;
}

void Inspection::setPort(Port* port) {
    processorIdentifier_ = port->getProcessor()->getIdentifier();
    portIdentifier_ = port->getIdentifier();
}

bool Inspection::samePort(Port* port) {
    if(processorIdentifier_ == port->getProcessor()->getIdentifier()
       && portIdentifier_ == port->getIdentifier()) {
        return true;
    } else {
        return false;
    }
}

void Inspection::resetPort() {
    processorIdentifier_ = "";
    portIdentifier_ = "";
}

} // namespace

