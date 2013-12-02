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

#include <inviwo/core/study/studyparameterlist.h>
#include <algorithm>

namespace inviwo {

StudyParameterList::StudyParameterList() {
    parameterProperties_ = new std::map<std::string, std::vector<Property*> >();
}

StudyParameterList::~StudyParameterList() { 
    clearAllParameters();
    delete parameterProperties_;
}

bool StudyParameterList::isParameterAdded(Property* property) {
    std::map<std::string, std::vector<Property*> >::iterator mapIt;
    for (mapIt = parameterProperties_->begin(); mapIt!=parameterProperties_->end(); mapIt++) {   
        std::vector<Property*> properties = mapIt->second;
        if (std::find(properties.begin(), properties.end(), property)!=properties.end()) return true;
    }
    return false;
}

void StudyParameterList::addParameter(std::string workspaceFileName, Property* property) {
    if (!isParameterAdded(property)) {
        (*parameterProperties_)[workspaceFileName].push_back(property);
        notifyObservers();
    }
}

void StudyParameterList::removeParameter(Property* property) {
    std::map<std::string, std::vector<Property*> >::iterator mapIt;
    for (mapIt = parameterProperties_->begin(); mapIt!=parameterProperties_->end(); mapIt++) {   
        std::vector<Property*> properties = mapIt->second;
        for (size_t i=0; i<properties.size(); i++) {
            if (properties[i] == property) {
                mapIt->second.erase(mapIt->second.begin()+i);
                notifyObservers();            
            }
        }
    }    
}

void StudyParameterList::removeParameters(std::string workspaceFileName) {
    (*parameterProperties_)[workspaceFileName].clear();
    parameterProperties_->erase(workspaceFileName);
    notifyObservers(); 
}

void StudyParameterList::clearAllParameters() { 
    parameterProperties_->clear();
    notifyObservers();
}

std::map<std::string, std::vector<Property*> >* StudyParameterList::getParameterProperties() {return parameterProperties_;}

} // namespace