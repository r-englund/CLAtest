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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include "inviwo/core/io/ivfwritersettings.h"

namespace inviwo {

IvfWriterSettings::IvfWriterSettings() 
    : WriterSettings(),
      IvfBaseSettings()    
{}


void IvfWriterSettings::serialize(IvwSerializer& s) const {
    WriterSettings::serialize(s);
    IvfBaseSettings::serialize(s);
}

void IvfWriterSettings::deserialize(IvwDeserializer& d) {
    WriterSettings::deserialize(d);
    IvfBaseSettings::deserialize(d);
}

} // namespace
