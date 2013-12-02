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

#ifndef IVW_STUDY_PARAMETER_LIST_H
#define IVW_STUDY_PARAMETER_LIST_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/singleton.h>
#include <inviwo/core/util/observer.h>
#include <map>
#include <vector>

namespace inviwo {

class Property;
class ProcessorNetwork;

class IVW_CORE_API StudyParameterList : public Singleton<StudyParameterList>, public VoidObservable {
public:
    StudyParameterList();    
	~StudyParameterList();	
    bool isParameterAdded(Property* property);
    void addParameter(std::string workspaceFileName, Property* property);	
	void removeParameter(Property* property);	
	void removeParameters(std::string workspaceFileName);
    void clearAllParameters();
    std::map<std::string, std::vector<Property*> >* getParameterProperties();
private:
    std::map<std::string, std::vector<Property*> >* parameterProperties_;
};

} // namespace

#endif // IVW_STUDY_PARAMETER_LIST_H