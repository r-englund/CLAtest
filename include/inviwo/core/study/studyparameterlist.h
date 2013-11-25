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