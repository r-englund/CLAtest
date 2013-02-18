#ifndef IVW_BASEOPTIONPROPERTY_H
#define IVW_BASEOPTIONPROPERTY_H


#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>

namespace inviwo {

class IVW_CORE_API BaseOptionProperty : public StringProperty{
public:
    BaseOptionProperty(std::string identifier, std::string displayName,std::string value,int selectedOption)
        :StringProperty(identifier,displayName,value), 
        selectedOption_(selectedOption)
    {}

    std::vector<std::string> optionKeys_;
    int getSelectedOption(){return selectedOption_;};
    void setSelectedOption(int tmp){ selectedOption_=tmp;};
    private:
        int selectedOption_;
};





template<typename T>
class IVW_CORE_API TemplatedOptionProperty : public BaseOptionProperty {

public:
    TemplatedOptionProperty(std::string identifier, std::string displayName,std::string value, int selectedOption);
    virtual void addOption(std::string optionName,T optionValue);
    virtual std::vector< std::pair<std::string, T> > getOptions();



private:
    std::vector< std::pair<std::string, T> > optionVector_;

};

template <typename T>
TemplatedOptionProperty<T>::TemplatedOptionProperty(std::string identifier, std::string displayName, std::string value, int selectedOption)
    : BaseOptionProperty(identifier,displayName,value, selectedOption)

{}

template<typename T>
void TemplatedOptionProperty<T>::addOption(std::string optionName,T optionValue){
    optionVector_.push_back(std::make_pair(optionName,optionValue));
    optionKeys_.push_back(optionName);
}
template<typename T>
std::vector< std::pair<std::string, T> > TemplatedOptionProperty< T >::getOptions(){
    return optionVector_;
}

} // namespace

#endif // IVW_BASEOPTIONPROPERTY_H