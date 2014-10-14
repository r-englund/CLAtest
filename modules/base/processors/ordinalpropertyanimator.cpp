/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact: Peter Steneteg
 *
 *********************************************************************************/

#include "ordinalpropertyanimator.h"
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

ProcessorClassIdentifier(OrdinalPropertyAnimator, "org.inviwo.Ordinal Property Animator");
ProcessorDisplayName(OrdinalPropertyAnimator,  "Property Animator");
ProcessorTags(OrdinalPropertyAnimator, Tags::None);
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

