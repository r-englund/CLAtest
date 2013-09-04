#include <inviwo/core/processors/progressbar.h>


namespace inviwo {

ProgressBar::ProgressBar() :
    progress_(0.0f), beginLoopProgress_(-1.0f)
{}

ProgressBar::~ProgressBar() {}



void ProgressBar::updateProgress(float progress) {
    if(visible_) {
        ivwAssert(progress>=progress_, "Progress should always increase");
        ivwAssert(progress>=0.0f&&progress<=1.0, "Progress out of bounds.");
        progress_ = progress;
        notifyProgressChanged();    
    }
}

void ProgressBar::updateProgressLoop(size_t loopVar, size_t maxLoopVar, float endLoopProgress) {
    if(visible_) {
        if (beginLoopProgress_<=0.0f)
            beginLoopProgress_ = progress_;
        float normalizedLoopVar = static_cast<float>(loopVar)/static_cast<float>(maxLoopVar);
        progress_ = beginLoopProgress_+normalizedLoopVar*(endLoopProgress-beginLoopProgress_);
        if (loopVar == maxLoopVar)
            beginLoopProgress_ = -1.0f;
        notifyProgressChanged();
    }
}

void ProgressBar::serialize(IvwSerializer& s) const {
    s.serialize("visible", visible_);
}

void ProgressBar::deserialize(IvwDeserializer& d) {
    d.deserialize("visible", visible_);
}


} // namespace
