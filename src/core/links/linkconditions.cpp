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

#include <inviwo/core/links/linkconditions.h>

namespace inviwo {

bool SimpleLinkCondition::canLink(Property* src, Property *dst) {
    if (*src == *dst) return true;
	return false;
}

bool AutoLinkCondition::canLink(Property* src, Property *dst) {
    //does properties have same class names
    if (SimpleLinkCondition::canLink(src, dst)) {
		bool canLink = false;
		//does properties have same identifiers
		if (src->getIdentifier() == dst->getIdentifier()) {                    
			canLink = true;
		}
		else {

			//conversion to lower case
			std::string srcIdentifier = src->getIdentifier();
			std::transform(srcIdentifier.begin(), srcIdentifier.end(), srcIdentifier.begin(), tolower);

			std::string dstIdentifier = dst->getIdentifier();
			std::transform(dstIdentifier.begin(), dstIdentifier.end(), dstIdentifier.begin(), tolower);

			std::string srcClassName = src->getClassName();
			std::transform(srcClassName.begin(), srcClassName.end(), srcClassName.begin(), tolower);

			std::string dstClassName = dst->getClassName();
			std::transform(dstClassName.begin(), dstClassName.end(), dstClassName.begin(), tolower);

			//does class name occurs in identifiers
			if (srcIdentifier.find(dstClassName)!=std::string::npos &&
				dstIdentifier.find(srcClassName)!=std::string::npos)
				canLink = true;

			//does identifier occur in other identifier
			if (srcIdentifier.find(dstIdentifier)!=std::string::npos ||
				dstIdentifier.find(srcIdentifier)!=std::string::npos)
				canLink = true;
		}   
			
		return canLink;
	}
	return false;
}

}