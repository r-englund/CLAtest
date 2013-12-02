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

#include <inviwo/core/io/datareader.h>

namespace inviwo {

    DataReader::DataReader() : extensions_() {}

    DataReader::DataReader( const DataReader& rhs ) : extensions_(rhs.extensions_){    
    }

    DataReader& DataReader::operator=( const DataReader& that ){
        if (this != &that) {
            extensions_.clear();
            for(std::vector<FileExtension>::const_iterator it = that.getExtensions().begin();
                it != that.getExtensions().end(); ++it){
                    extensions_.push_back(*it);
            }
        }
        return *this;
    }

    const std::vector<FileExtension>& DataReader::getExtensions() const {
        return extensions_;
    }
    void DataReader::addExtension( FileExtension ext ){
        extensions_.push_back(ext);
    }



} // namespace
