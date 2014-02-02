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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/io/datawriter.h>

namespace inviwo {
DataWriterException::DataWriterException(const std::string& message)
    : Exception(message) {}


DataWriter::DataWriter() : overwrite_(false), extensions_() {}

DataWriter::DataWriter(const DataWriter& rhs)
    : overwrite_(rhs.overwrite_)
    , extensions_(rhs.extensions_) {
}

DataWriter& DataWriter::operator=(const DataWriter& that) {
    if (this != &that) {
        overwrite_ = that.overwrite_;
        extensions_.clear();

        for (std::vector<FileExtension>::const_iterator it = that.getExtensions().begin();
             it != that.getExtensions().end(); ++it)
            extensions_.push_back(*it);
    }

    return *this;
}

const std::vector<FileExtension>& DataWriter::getExtensions() const {
    return extensions_;
}
void DataWriter::addExtension(FileExtension ext) {
    extensions_.push_back(ext);
}

bool DataWriter::getOverwrite() const {
    return overwrite_;
}
void DataWriter::setOverwrite(bool val) {
    overwrite_ = val;
}

} // namespace
