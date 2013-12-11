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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#ifndef IVW_PROPERTYSEMANTCIS_H
#define IVW_PROPERTYSEMANTCIS_H

namespace inviwo {
class IVW_CORE_API PropertySemantics {

public:
    enum Type { Default =0,
                Color =1,
                LightPosition =2,
                Editor=3,
                Collapsible=4,
				Shader=5};
};
} //Namespace

#endif //IVW_PROPERTYSEMANTCIS_H