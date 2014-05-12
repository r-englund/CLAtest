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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_VOLUMEREPRESENTATION_H
#define IVW_VOLUMEREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

class DataRepresentation;
class Volume;

class IVW_CORE_API VolumeRepresentation : public DataRepresentation {

    friend class Volume;

public:
    /// struct volume borders
    struct VolumeBorders {
        uvec3 llf;
        uvec3 urb;
        size_t numVoxels;
        bool hasBorder;

        VolumeBorders() : llf(uvec3(0,0,0)), urb(uvec3(0,0,0)), numVoxels(0), hasBorder(false) {}
        VolumeBorders(size_t front, size_t back, size_t left, size_t right, size_t lower, size_t upper) : llf(uvec3(front, left, lower)),
            urb(uvec3(back, right, upper)) {}
        VolumeBorders(const uvec3& llfBorder, const uvec3& urbBorder) : llf(llfBorder), urb(urbBorder) {}
        bool operator== (const VolumeBorders& vb) const { return (llf == vb.llf && urb == vb.urb);}
        bool operator!= (const VolumeBorders& vb) const { return (llf != vb.llf || urb != vb.urb);}
    };

    VolumeRepresentation(uvec3 dimension);
    VolumeRepresentation(uvec3 dimension, const DataFormatBase* format);
    VolumeRepresentation(uvec3 dimension, const DataFormatBase* format, VolumeBorders border);
    VolumeRepresentation(const VolumeRepresentation& rhs);
    VolumeRepresentation& operator=(const VolumeRepresentation& that);
    virtual VolumeRepresentation* clone() const;
    virtual ~VolumeRepresentation();

    virtual void performOperation(DataOperation*) const {};
    // Removes old data and reallocate for new dimension.
    // Needs to be overloaded by child classes.
    virtual void setDimension(uvec3 dimensions) { dimensions_ = dimensions;}

    const VolumeBorders& getBorder() const {return borders_;}
    virtual uvec3 getBorderLLF() const {return borders_.llf;}
    virtual uvec3 getBorderURB() const {return borders_.urb;}
    const uvec3& getDimension() const {return dimensions_;}
    uvec3 getDimensionWithBorder() const { return dimensions_+getBorderLLF()+getBorderURB(); }
    bool hasBorder() const {return borders_.hasBorder;}

protected:
    uvec3 dimensions_;
    VolumeBorders borders_;
};

} // namespace

#endif // IVW_VOLUMEREPRESENTATION_H
