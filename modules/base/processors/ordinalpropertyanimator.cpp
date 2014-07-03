#include "ordinalpropertyanimator.h"

namespace inviwo {

ProcessorClassName(OrdinalPropertyAnimator, "Ordinal Property Animator");
ProcessorCategory(OrdinalPropertyAnimator, "Various");
ProcessorCodeState(OrdinalPropertyAnimator, CODE_STATE_EXPERIMENTAL);


OrdinalPropertyAnimator::OrdinalPropertyAnimator()
    : Processor()
    , type_("property", "Property")
    , delay_("delay", "Delay (ms)", 50, 1, 10000, 1)
    , pbc_("pbc", "Periodic", true)
    , active_("active", "Active", true) {
    
    timer_ = InviwoApplication::getPtr()->createTimer();

    timer_->setElapsedTimeCallback(this, &OrdinalPropertyAnimator::timerEvent);

    delay_.onChange(this, &OrdinalPropertyAnimator::updateTimerInterval);

    properties_.push_back(new PrimProp<float>("FloatProperty", "FloatProperty"));
    properties_.push_back(new VecProp<vec2>("FloatVec2Property", "FloatVec2Property"));
    properties_.push_back(new VecProp<vec3>("FloatVec3Property", "FloatVec3Property"));
    properties_.push_back(new VecProp<vec4>("FloatVec4Property", "FloatVec4Property"));
    properties_.push_back(new PrimProp<double>("DoubleProperty","DoubleProperty"));
    properties_.push_back(new VecProp<dvec2>("DoubleVec2Property", "DoubleVec2Property"));
    properties_.push_back(new VecProp<dvec3>("DoubleVec3Property", "DoubleVec3Property"));
    properties_.push_back(new VecProp<dvec4>("DoubleVec4Property", "DoubleVec4Property"));
    properties_.push_back(new PrimProp<int>("IntProperty", "IntProperty"));
    properties_.push_back(new VecProp<ivec2>("IntVec2Property", "IntVec2Property"));
    properties_.push_back(new VecProp<ivec3>("IntVec3Property", "IntVec3Property"));
    properties_.push_back(new VecProp<ivec4>("IntVec4Property", "IntVec4Property"));

    addProperty(type_);
    addProperty(active_);
    addProperty(delay_);
    addProperty(pbc_);

    std::vector<BaseProp*>::const_iterator itBegin = properties_.begin(); 
    for (std::vector<BaseProp*>::const_iterator it = itBegin; it != properties_.end(); ++it) {
        type_.addOption((*it)->classname_, (*it)->displayName_, std::distance(itBegin, it));
        Property* prop = (*it)->getProp();
        Property* delta = (*it)->getDelta();

        addProperty(prop);
        addProperty(delta);
        prop->setVisible(false);
        delta->setVisible(false);
    }
    type_.setSelectedIndex(0);
    type_.setCurrentStateAsDefault();
    
    type_.onChange(this, &OrdinalPropertyAnimator::changeProperty);

    active_.onChange(this, &OrdinalPropertyAnimator::changeActive);

    changeProperty();
    updateTimerInterval();
}

OrdinalPropertyAnimator::~OrdinalPropertyAnimator() {
    timer_->stop();
    delete timer_;

    for (std::vector<BaseProp*>::const_iterator it = properties_.begin(); it != properties_.end(); ++it) {
        delete *it;
    }
    properties_.clear();
}

void OrdinalPropertyAnimator::initialize() {
    changeProperty();
    updateTimerInterval();
}

void OrdinalPropertyAnimator::deinitialize() {
    timer_->stop();
}

void OrdinalPropertyAnimator::process() {
    if (!active_.get()) timer_->stop();
}

void OrdinalPropertyAnimator::updateTimerInterval() {
    timer_->stop();
    if(active_.get())
        timer_->start(delay_.get());
}

void OrdinalPropertyAnimator::timerEvent() {
    int ind = type_.get();
    properties_[ind]->update(pbc_.get());
}

void OrdinalPropertyAnimator::changeProperty() {
    int ind = type_.get();
    
    for (std::vector<BaseProp*>::const_iterator it = properties_.begin(); it != properties_.end(); ++it) {
        Property* prop = (*it)->getProp();
        Property* delta = (*it)->getDelta();
        prop->setVisible(false);
        delta->setVisible(false);
    }
    
    properties_[ind]->getProp()->setVisible(true);
    properties_[ind]->getDelta()->setVisible(true);
}

void OrdinalPropertyAnimator::changeActive() {
    if (active_.get()) {
        updateTimerInterval();
    } else {
        timer_->stop();
    }
}

} // namespace

