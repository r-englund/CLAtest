#ifndef IVW_STUDY_PARAMETER_LIST_H
#define IVW_STUDY_PARAMETER_LIST_H

#include <inviwo/core/util/singleton.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class IVW_CORE_API StudyParameterList : public Singleton<StudyParameterList>, public VoidObservable {
public:
    StudyParameterList() {}    
	~StudyParameterList();	
    bool isParameterAdded(Property* property);
	void addParameter(ProcessorNetwork* network, Property* property);	
	void removeParameter(Property* property);	
	void removeParameters(ProcessorNetwork* network);	
    void clearAllParameters();
    std::map<ProcessorNetwork*, std::vector<Property*> > getParameters();
private:
	std::map<ProcessorNetwork*, std::vector<Property*> > parameterProperties_;
	std::map<Property*, Property* > parameterConstraints_;
};

} // namespace

#endif // IVW_STUDY_PARAMETER_LIST_H