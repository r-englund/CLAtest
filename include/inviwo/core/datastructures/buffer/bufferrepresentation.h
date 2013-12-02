/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_BUFFER_REPRESENTATION_H
#define IVW_BUFFER_REPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/buffer/buffer.h>

namespace inviwo {

class IVW_CORE_API BufferRepresentation : public DataRepresentation {

public:
    BufferRepresentation(size_t size, const DataFormatBase* format = DataFormatBase::get(), BufferType type = POSITION_ATTRIB, BufferUsage usage = STATIC);
    BufferRepresentation(const BufferRepresentation& rhs);
    virtual ~BufferRepresentation() {};
    virtual void performOperation(DataOperation*) const {};
    virtual void setSize(size_t size) {  size_ = size; }
    virtual void resize(size_t size) { size_ = size; }

    virtual BufferRepresentation* clone() const = 0;
    virtual std::string getClassName() const { return "BufferRepresentation"; }
    /**
     * Return the number of elements in the buffer.
     * 
     * @return Number of elements in the buffer
     */
    size_t getSize() const { return size_; }

    /**
     * Return size of buffer element in bytes.
     * 
     * @return Size of element in bytes.
     */
    virtual size_t getSizeOfElement() const { return getDataFormat()->getBytesStored(); };

    BufferType getBufferType() const { return type_; }
    BufferUsage getBufferUsage() const { return usage_; }

protected:
    size_t size_;
    BufferType type_;
    BufferUsage usage_;
};

} // namespace

#endif // IVW_BUFFER_REPRESENTATION_H
