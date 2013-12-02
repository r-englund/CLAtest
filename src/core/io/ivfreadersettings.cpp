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

#include "inviwo/core/io/ivfreadersettings.h"

namespace inviwo {

IvfReaderSettings::IvfReaderSettings() 
    : ReaderSettings(),
      IvfBaseSettings()
{}

void IvfReaderSettings::serialize(IvwSerializer& s) const {
    ReaderSettings::serialize(s);
    IvfBaseSettings::serialize(s);
}

void IvfReaderSettings::deserialize(IvwDeserializer& d) {
    ReaderSettings::deserialize(d);
    IvfBaseSettings::deserialize(d);
}

} // namespace
