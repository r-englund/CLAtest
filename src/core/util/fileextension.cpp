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

#include <inviwo/core/util/fileextension.h>

#include <inviwo/core/util/singleton.h>

namespace inviwo {
std::vector<SingeltonBase*> SingeltonBase::instances_;

FileExtension::FileExtension()
    : extension_("txt")
    , description_("Textfile") {};
FileExtension::FileExtension(std::string extension, std::string description)
    : extension_(extension)
    , description_(description) {};

} // namespace

