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
 *********************************************************************************/

#ifndef IVW_GEOMETRYPORT_H
#define IVW_GEOMETRYPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/ports/multidatainport.h>
#include <inviwo/core/datastructures/geometry/geometry.h>

namespace inviwo {

class IVW_CORE_API GeometryInport : public DataInport<Geometry> {

public:
    GeometryInport(std::string identifier,
                   InvalidationLevel invalidationLevel=INVALID_OUTPUT);
    virtual ~GeometryInport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
    static uvec3 colorCode;
    virtual std::string getClassIdentifier() const {return "org.inviwo.GeometryInport";}
};

class IVW_CORE_API GeometryMultiInport : public MultiDataInport<Geometry> {

public:
    GeometryMultiInport(std::string identifier);
    virtual ~GeometryMultiInport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
    virtual std::string getClassIdentifier() const {return "org.inviwo.GeometryMultiInport";}
};

class IVW_CORE_API GeometryOutport : public DataOutport<Geometry> {

public:
    GeometryOutport(std::string identifier,
                    InvalidationLevel invalidationLevel=INVALID_OUTPUT);
    virtual ~GeometryOutport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
    virtual std::string getClassIdentifier() const {return "org.inviwo.GeometryOutport";}
};

} // namespace

#endif // IVW_GEOMETRYPORT_H
