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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/io/serialization/ivwserializeconstants.h>

namespace inviwo {

const std::string IvwSerializeConstants::XML_VERSION="1.0";
const std::string IvwSerializeConstants::INVIWO_TREEDATA="InviwoTreeData";
const std::string IvwSerializeConstants::INVIWO_VERSION="1.0";
const std::string IvwSerializeConstants::NETWORK_VERSION="1.0";
const std::string IvwSerializeConstants::VERSION="version";
const std::string IvwSerializeConstants::EDIT_COMMENT=" Don't edit the following code ";
const std::string IvwSerializeConstants::ID_ATTRIBUTE="id";
const std::string IvwSerializeConstants::REF_ATTRIBUTE="reference";
const std::string IvwSerializeConstants::VERSION_ATTRIBUTE="version";
const std::string IvwSerializeConstants::CONTENT_ATTRIBUTE="content";
const std::string IvwSerializeConstants::TYPE_ATTRIBUTE="type";
const std::string IvwSerializeConstants::KEY_ATTRIBUTE="key";
const std::string IvwSerializeConstants::COLOR_R_ATTRIBUTE="r";
const std::string IvwSerializeConstants::COLOR_G_ATTRIBUTE="g";
const std::string IvwSerializeConstants::COLOR_B_ATTRIBUTE="b";
const std::string IvwSerializeConstants::COLOR_A_ATTRIBUTE="a";
const std::string IvwSerializeConstants::VECTOR_X_ATTRIBUTE="x";
const std::string IvwSerializeConstants::VECTOR_Y_ATTRIBUTE="y";
const std::string IvwSerializeConstants::VECTOR_Z_ATTRIBUTE="z";
const std::string IvwSerializeConstants::VECTOR_W_ATTRIBUTE="w";
const unsigned int IvwSerializeConstants::STRINGSTREAM_PRECISION=8;

const std::string IvwSerializeConstants::PROPERTY_ATTRIBUTE_1="identifier";
const std::string IvwSerializeConstants::PROPERTY_ATTRIBUTE_2="displayName";
const std::string IvwSerializeConstants::PROCESSOR_ATTRIBUTE_1="identifier";
const std::string IvwSerializeConstants::PROCESSOR_ATTRIBUTE_2="displayName";

bool IvwSerializeConstants::isReversvedAttribute(const std::string key) {
    if (   key == PROPERTY_ATTRIBUTE_1
        || key == PROPERTY_ATTRIBUTE_2
        || key == PROCESSOR_ATTRIBUTE_1
        || key == PROCESSOR_ATTRIBUTE_2 ) {
        return true;
    }
    return false;
}

} //namespace
