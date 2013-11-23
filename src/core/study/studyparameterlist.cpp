
#include <inviwo/core/study/studyparameterlist.h>
#include <algorithm>

namespace inviwo {

StudyParameterList::StudyParameterList() {
    parameterProperties_ = new std::map<ProcessorNetwork*, std::vector<Property*> >();
    parameterConstraints_ = new std::map<Property*, Property* >();
}

StudyParameterList::~StudyParameterList() { 
    clearAllParameters();
    delete parameterProperties_;
    delete parameterConstraints_;
}

bool StudyParameterList::isParameterAdded(Property* property) {
    std::map<ProcessorNetwork*, std::vector<Property*> >::iterator mapIt;
    for (mapIt = parameterProperties_->begin(); mapIt!=parameterProperties_->end(); mapIt++) {   
        std::vector<Property*> properties = mapIt->second;
        if (std::find(properties.begin(), properties.end(), property)!=properties.end()) return true;
    }
    return false;
}

void StudyParameterList::addParameter(ProcessorNetwork* network, Property* property) {
    if (!isParameterAdded(property)) {
        (*parameterProperties_)[network].push_back(property);
        notifyObservers();
    }
}

void StudyParameterList::removeParameter(Property* property) {    
    std::map<ProcessorNetwork*, std::vector<Property*> >::iterator mapIt;
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

void StudyParameterList::removeParameters(ProcessorNetwork* network) {
    (*parameterProperties_)[network].clear();
    parameterProperties_->erase(network);
    notifyObservers(); 
}

void StudyParameterList::clearAllParameters() { 
    parameterProperties_->clear();
    notifyObservers();
}

std::map<ProcessorNetwork*, std::vector<Property*> >* StudyParameterList::getParameters() {return parameterProperties_;}

} // namespace