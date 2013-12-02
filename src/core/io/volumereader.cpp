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

#include "inviwo/core/io/volumereader.h"

namespace inviwo {

VolumeReader::VolumeReader() : DataReaderType<Volume>() {}

VolumeReader::VolumeReader( const VolumeReader& rhs ) : DataReaderType<Volume>(rhs){ }

VolumeReader& VolumeReader::operator=( const VolumeReader& that ){
    if (this != &that) {
        DataReaderType<Volume>::operator=(that);
    }
    return *this;
}


} // namespace
