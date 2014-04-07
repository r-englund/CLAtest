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
 * Main file authors: Martin Falk
 *
 *********************************************************************************/

#ifndef IVW_VECTORDATA_PORT_H
#define IVW_VECTORDATA_PORT_H

#include <modules/mesh/meshmoduledefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <vector>
#include <typeinfo>


namespace inviwo {

class IVW_CORE_API VectorDataPortColor {
public:
    static uvec3 colorCode;
};

template <typename T>
class VectorData : public DataBase {
public:
    VectorData() {}
    VectorData(const DataBase& rhs) : DataBase(rhs),vector(rhs.vector) {}
    VectorData& operator=(const VectorData& rhs) { 
        if (this != rhs) {
            this = rhs;
        }
    }
    virtual VectorData* clone() const { return new VectorData(*this); }
    virtual ~VectorData() {}

    std::vector<T> vector;

    std::string getDataInfo() const {
        std::ostringstream stream;
        stream << "Type:   vector\n"
            << "Format: " << typeid(T).name() << "\n"
            << "Length: " << vector.size() << "\n";
        return stream.str();
    }
};

template <typename Type>
class VectorDataPort : public DataInport<VectorData<Type> > {

public:
    VectorDataPort(std::string identifier,
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT)
        : DataInport<VectorData<Type> >(identifier, invalidationLevel) {}
    virtual ~VectorDataPort() {}

    void initialize() {}
    void deinitialize() {}

    uvec3 getColorCode() const { return VectorDataPortColor::colorCode; }
    virtual std::string getClassName() const { return "VectorDataInport"; }
};

template <typename Type>
class VectorDataOutport : public DataOutport<VectorData<Type> > {

public:
    VectorDataOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT)
        : DataOutport<VectorData<Type> >(identifier, invalidationLevel) 
    {
            this->setData(new VectorData<Type>, true);
    }
    virtual ~VectorDataOutport() {}

    void initialize() {}
    void deinitialize() {}

    uvec3 getColorCode() const { return VectorDataPortColor::colorCode; }
    virtual std::string getClassName() const { return "VectorDataOutport"; }
};

} // namespace

#endif // IVW_VECTORDATA_PORT_H
