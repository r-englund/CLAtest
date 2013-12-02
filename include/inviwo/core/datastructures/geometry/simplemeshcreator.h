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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_SIMPLEMESHCREATOR_H
#define IVW_SIMPLEMESHCREATOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/simplemesh.h>

namespace inviwo {

class IVW_CORE_API SimpleMeshCreator {

public:
    static SimpleMesh* rectangularPrism(vec3 posLlf, vec3 posUrb, vec3 texCoordLlf, 
                                        vec3 texCoordUrb, vec4 colorLlf, vec4 colorUrb);
	static SimpleMesh* parallelepiped(glm::vec3 pos, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, 	
									   glm::vec3 tex, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, 	
									   glm::vec4 col, glm::vec4 c1, glm::vec4 c2, glm::vec4 c3); 	

	static SimpleMesh* rectangle(glm::vec3 posLl, glm::vec3 posUr);

    static SimpleMesh* sphere(float radius, unsigned int numLoops, unsigned int segmentsPerLoop);
};

} // namespace

#endif // IVW_SIMPLEMESHCREATOR_H
