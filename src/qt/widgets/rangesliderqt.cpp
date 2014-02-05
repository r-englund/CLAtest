 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/rangesliderqt.h>
#include <QFrame>

namespace inviwo {

RangeSliderQt::RangeSliderQt(Qt::Orientation orientation, QWidget* parent) : QSplitter(orientation, parent) {
    QFrame* left = new QFrame(this);
    QFrame* middle = new QFrame(this);
    QFrame* right = new QFrame(this);
    addWidget(left);
    addWidget(middle);
    addWidget(right);
    left->setStyleSheet(QString(
                            "QSplitter QFrame {"
                            "  border-top-right-radius: 0px;"
                            "  border-bottom-right-radius: 0px;"
                            "}"
                        ));
    right->setStyleSheet(QString(
                             "QSplitter QFrame {"
                             "  border-top-left-radius: 0px;"
                             "  border-bottom-left-radius: 0px;"
                             "}"
                         ));

    if (orientation == Qt::Horizontal) setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    else setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    connect(this, SIGNAL(splitterMoved(int, int)), this, SLOT(updateSplitterPosition(int, int)));
    range_[0] = 0;
    range_[1] = 0;
    value_[0] = 0;
    value_[1] = 0;
    internalRange_[0] = 0;
    internalRange_[1] = 0;
    internalValue_[0] = 0;
    internalValue_[1] = 0;
    getRange(1, &internalRange_[0], &internalValue_[1]);
    getRange(2, &internalValue_[0], &internalRange_[1]);
}

RangeSliderQt::~RangeSliderQt() {}

int RangeSliderQt::minValue() {
    return value_[0];
}

int RangeSliderQt::maxValue() {
    return value_[1];
}

int RangeSliderQt::minRange() {
    return range_[0];
}

int RangeSliderQt::maxRange() {
    return range_[1];
}

void RangeSliderQt::setValue(int minVal, int maxVal) {
    setMinValue(minVal);
    setMaxValue(maxVal);
}

void RangeSliderQt::setMinValue(int minVal) {
    if (value_[0] != minVal) {
        value_[0] = minVal;
        internalValue_[0] = closestLegalPosition(fromExternalToInternal(minVal), 1);
        moveSplitter(internalValue_[0], 1);
    }
}

void RangeSliderQt::setMaxValue(int maxVal) {
    if (value_[1] != maxVal) {
        value_[1] = maxVal;
        internalValue_[1] = closestLegalPosition(fromExternalToInternal(maxVal)+getHandleWidth(), 2);
        moveSplitter(internalValue_[1], 2);
    }
}

void RangeSliderQt::setRange(int minR, int maxR) {
    setMinRange(minR);
    setMaxRange(maxR);
}

void RangeSliderQt::setMinRange(int minR) {
    range_[0] = minR;

    if (minR>value_[0])
        setMinValue(minR);
}

void RangeSliderQt::setMaxRange(int maxR) {
    range_[1] = maxR;

    if (maxR<value_[1])
        setMaxValue(maxR);
}

//Index 1 = Min, Index 2 = Max
int RangeSliderQt::constrainValues(int lastIdxChanged) {
    if (lastIdxChanged == 2) {
        internalValue_[1] -= getHandleWidth();

        if (internalValue_[0] > internalValue_[1]) {
            internalValue_[0] = internalValue_[1];
            return 1;
        }
    }
    else if (internalValue_[0] > internalValue_[1]) {
        internalValue_[1] = internalValue_[0];
        return 2;
    }

    return -1;
}

void RangeSliderQt::resizeEvent(QResizeEvent* event) {
    QSplitter::resizeEvent(event);
    getRange(1, &internalRange_[0], &internalValue_[1]);
    getRange(2, &internalValue_[0], &internalRange_[1]);

    if (internalValue_[0]<0 || internalValue_[1]<0)
        return;

    internalRange_[1] -= getHandleWidth();
    internalValue_[0] = closestLegalPosition(fromExternalToInternal(value_[0]), 1);
    moveSplitter(internalValue_[0], 1);
    internalValue_[1] = closestLegalPosition(fromExternalToInternal(value_[1])+getHandleWidth(), 2);
    moveSplitter(internalValue_[1], 2);
}

int RangeSliderQt::fromInternalToExternal(int val) {
    return (val*((static_cast<float>(maxRange()-minRange()))/(static_cast<float>(internalRange_[1]-internalRange_[0]))))+minRange();
}

int RangeSliderQt::fromExternalToInternal(int val) {
    return (val-minRange())*((static_cast<float>(internalRange_[1]-internalRange_[0]))/(static_cast<float>(maxRange()-minRange())));
}

int RangeSliderQt::getHandleWidth() {
    return handleWidth();
}

//Index 1 = Min, Index 2 = Max
void RangeSliderQt::updateSplitterPosition(int pos, int idx) {
    //Return if invalid position
    if (pos<0)
        return;

    int loc = idx-1;
    //Check if interaction came from handle
    bool changedFromHandle = (internalValue_[loc] == pos ? false : true);
    internalValue_[loc] = pos;
    //Constrain min/max values against each other
    int otherIdx = constrainValues(idx);
    int otherLoc = otherIdx-1;

    //Change value if it was changed from handle
    if (changedFromHandle)
        value_[loc] = fromInternalToExternal(internalValue_[loc]);

    //Perform change of other handle if constrain was performed
    if (otherIdx > 0)
        value_[otherLoc] = fromInternalToExternal(internalValue_[otherLoc]);

    //Emit
    emit valuesChanged(value_[0], value_[1]);
}

} // namespace inviwo
