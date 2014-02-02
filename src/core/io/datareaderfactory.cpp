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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

DataReaderFactory::DataReaderFactory() {
}


void DataReaderFactory::registerObject(DataReader* reader) {
    for (std::vector<FileExtension>::const_iterator it = reader->getExtensions().begin();
         it != reader->getExtensions().end(); ++it) {
        if (readerForExtension_.find(it->extension_) == readerForExtension_.end())
            readerForExtension_.insert(std::make_pair(it->extension_, reader));
    }
}



} // namespace

