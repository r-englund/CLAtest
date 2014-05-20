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

#ifndef IVW_VOLUMESOURCE_H
#define IVW_VOLUMESOURCE_H

#include <modules/base/basemoduledefine.h>
#include <modules/base/processors/datasource.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/processors/progressbarowner.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/minmaxproperty.h>
#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/ordinalproperty.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSource : public DataSource<Volume, VolumeOutport> {
public:
    VolumeSource();
    ~VolumeSource();

    InviwoProcessorInfo();

    void onOverrideChange();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    virtual void dataLoaded(Volume* data);
    virtual void process();

private:

    void saveState();
    void restoreState();

    struct VolumeSourceState {
    public:
        VolumeSourceState();
        DoubleMinMaxProperty dataRange;
        DoubleMinMaxProperty valueRange;
        StringProperty valueUnit;
        BoolProperty overRideDefaults;
        FloatVec3Property lengths;
        FloatVec3Property angles;
        FloatVec3Property offset;

        static void assignStateIfChanged(const DoubleMinMaxProperty& in, 
                                         const DoubleMinMaxProperty& ref,
                                         DoubleMinMaxProperty& out);

        static void assignStateIfChanged(const StringProperty& in,
                                         const StringProperty& ref,
                                         StringProperty& out);

        static void assignStateIfChanged(const BoolProperty& in,
                                         const BoolProperty& ref,
                                         BoolProperty& out);

        static void assignStateIfChanged(const FloatVec3Property& in,
                                         const FloatVec3Property& ref,
                                         FloatVec3Property& out);
    };
    
    DoubleMinMaxProperty dataRange_;
    DoubleMinMaxProperty valueRange_;
    StringProperty valueUnit_;
    BoolProperty overRideDefaults_;
    FloatVec3Property lengths_;
    FloatVec3Property angles_;
    FloatVec3Property offset_;

    vec3 overrideLengths_;
    vec3 overrideAngles_;
    vec3 overrideOffset_;
    
    // Readonly only use to show information
    StringProperty dimensions_;
    StringProperty format_;

    VolumeSourceState oldState;

    bool isDeserializing_;
};

} // namespace

#endif // IVW_VOLUMESOURCE_H
