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
 * Main file authors: Peter Steneteg
 *
 *********************************************************************************/

#ifndef IVW_VERSIONCONVERTER_H
#define IVW_VERSIONCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API VersionConverter { 
public:
    VersionConverter();
    virtual ~VersionConverter(){}
    virtual bool convert(TxElement* root) = 0;
};


template <typename T>
class TraversingVersionConverter : public VersionConverter {
public:
    typedef void (T::*ConvertNodeFunPtr)(TxElement*);

    TraversingVersionConverter(T* obj, ConvertNodeFunPtr fPtr);
    virtual ~TraversingVersionConverter(){}
    virtual bool convert(TxElement* root);

private:
    void traverseNodes(TxElement* node, ConvertNodeFunPtr update);
    T* obj_;
    ConvertNodeFunPtr fPtr_;
};

template <typename T>
bool inviwo::TraversingVersionConverter<T>::convert(TxElement* root) {
    traverseNodes(root, fPtr_); 
    return true;
}

template <typename T>
inviwo::TraversingVersionConverter<T>::TraversingVersionConverter(T* obj, ConvertNodeFunPtr fPtr) 
    : VersionConverter()
    , obj_(obj)
    , fPtr_(fPtr) {
}

template <typename T>
void inviwo::TraversingVersionConverter<T>::traverseNodes(TxElement* node, ConvertNodeFunPtr update) {
    (obj_->*fPtr_)(node);
    ticpp::Iterator<ticpp::Element> child;
    for (child = child.begin(node); child != child.end(); child++) {
        traverseNodes(child.Get(), update);
    }
}



} // namespace

#endif // IVW_VERSIONCONVERTER_H

