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

#ifndef IVW_DATAREADER_H
#define IVW_DATAREADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include "inviwo/core/datastructures/data.h"
#include <inviwo/core/util/fileextension.h>
#include <inviwo/core/util/exception.h>

namespace inviwo {

class IVW_CORE_API DataReaderException : public Exception {
public:
    DataReaderException(const std::string& message = "");
    virtual ~DataReaderException() throw() {};
};


/** \brief A abstract base class for all file readers. 
 *
 */
class IVW_CORE_API DataReader {
public:
    DataReader();
    DataReader(const DataReader& rhs);
    DataReader& operator=(const DataReader& that);
    virtual DataReader* clone() const = 0;
    virtual ~DataReader() {};
    
    virtual Data* readMetaData(const std::string filePath) = 0;
    virtual void* readData() const = 0;
    virtual void readDataInto(void* dest) const = 0;

    const std::vector<FileExtension>& getExtensions() const;
    void addExtension(FileExtension ext);

protected:

private:
    std::vector<FileExtension> extensions_;
};

/** \brief Template base class for file readers designating what type of data object the reader returns. 
 *
 */
template <typename T>
class DataReaderType : public DataReader {
public:
    DataReaderType() : DataReader() {};
    DataReaderType(const DataReaderType& rhs) : DataReader(rhs) {};
    DataReaderType& operator=(const DataReaderType& that) {
        if (this != &that) {
            DataReader::operator=(that);
        }
        return *this;
    };
    virtual DataReaderType* clone() const = 0;
    virtual ~DataReaderType() {};

    virtual T* readMetaData(const std::string filePath) = 0;
    virtual void* readData() const = 0;
    virtual void readDataInto(void* dest) const = 0;
};

} // namespace

#endif // IVW_DATAREADER_H
    
    