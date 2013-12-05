/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_DATAWRITER_H
#define IVW_DATAWRITER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/util/fileextension.h>
#include <inviwo/core/util/exception.h>

namespace inviwo {


class IVW_CORE_API DataWriterException : public Exception {
public:
    DataWriterException(const std::string& message = "");
    virtual ~DataWriterException() throw() {};
};


class IVW_CORE_API DataWriter {

public:
    DataWriter();
    DataWriter(const DataWriter& rhs);
    DataWriter& operator=(const DataWriter& that);
    virtual DataWriter* clone() const = 0;
    virtual ~DataWriter() {};

    const std::vector<FileExtension>& getExtensions() const;
    void addExtension(FileExtension ext);

private:
    std::vector<FileExtension> extensions_;
};


template <typename T>
class DataWriterType : public DataWriter {
public:
    DataWriterType() : DataWriter() {};
    DataWriterType(const DataWriterType& rhs) : DataWriter(rhs) {};
    DataWriterType& operator=(const DataWriterType& that) {
        if (this != &that) {
            DataWriter::operator=(that);
        }
        return *this;
    };
    virtual DataWriterType* clone() const = 0;
    virtual ~DataWriterType() {};

    virtual void writeData(const T* data, const std::string filePath) const = 0;
};


} // namespace

#endif // IVW_DATAWRITER_H
