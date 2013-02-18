#ifndef IVW_BASEOPTIONPROPERTY_H
#define IVW_BASEOPTIONPROPERTY_H


#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>

namespace inviwo {

class IVW_CORE_API BaseOptionProperty : public StringProperty{
public:

    BaseOptionProperty(std::string identifier, std::string displayName,std::string value)
        :StringProperty(identifier,displayName,value)
    {}

    virtual std::vector< std::string > getOptionKeys()=0;
    virtual int getSelectedOption()=0;
    virtual void setSelectedOption( int )=0;
    virtual void updateValue()=0;

};


template<typename T>
class IVW_CORE_API TemplatedOptionProperty : public BaseOptionProperty {

public:
    TemplatedOptionProperty(std::string identifier, std::string displayName,std::string value, int selectedOption);

    virtual void addOption(std::string optionName,T optionValue);
    virtual std::vector< std::pair<std::string, T> > getOptions();

    std::vector< std::string > getOptionKeys();
    int getSelectedOption();
    void setSelectedOption( int );
    virtual void updateValue();

private:
    std::vector< std::pair<std::string, T> > optionVector_;
    int selectedOption_;
};


template <typename T>
TemplatedOptionProperty<T>::TemplatedOptionProperty(std::string identifier, std::string displayName, std::string value, int selectedOption)
    : BaseOptionProperty(identifier,displayName,value),
    selectedOption_(selectedOption)
{}

template<typename T>
void TemplatedOptionProperty<T>::addOption(std::string optionName,T optionValue){
    optionVector_.push_back(std::make_pair(optionName,optionValue));
}
template<typename T>
std::vector< std::pair<std::string, T> > TemplatedOptionProperty< T >::getOptions(){
    return optionVector_;
}

template<typename T>
std::vector< std::string > TemplatedOptionProperty<T>::getOptionKeys(){
    
    std::vector< std::string > ret;
    int size_ = optionVector_.size();
    for (int i=0; i<size_;i++){
        ret.push_back(optionVector_.at(i).first);
    }
    return ret;
}

template<typename T>
int TemplatedOptionProperty<T>::getSelectedOption(){
    return selectedOption_;
}
template<typename T>
void TemplatedOptionProperty<T>::setSelectedOption(int idx){
    selectedOption_ = idx;
    updateValue();
}
template<typename T>
void TemplatedOptionProperty<T>::updateValue(){
    int size_ = optionVector_.size();
    if(size_<=selectedOption_)
    set(optionVector_.at(selectedOption_).first);
}

} // namespace

#endif // IVW_BASEOPTIONPROPERTY_H