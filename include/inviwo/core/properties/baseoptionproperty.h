#ifndef IVW_BASEOPTIONPROPERTY_H
#define IVW_BASEOPTIONPROPERTY_H


#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {
 
class IVW_CORE_API BaseOptionProperty : public StringProperty{
public:

    BaseOptionProperty(std::string identifier, std::string displayName, std::string value,
        PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics::Type semantics = PropertySemantics::Default)
        :StringProperty(identifier,displayName,value,invalidationLevel,semantics)
    {}

    virtual std::vector< std::string > getOptionKeys()=0;
    virtual int getSelectedOption()=0;
    virtual void setSelectedOption( std::string )=0;
    virtual void updateValue( std::string )=0;

};
/** class TemplatedOptionProperty 
*   Template class for option properties 
*  
* @see OptionProperties 
* @see BaseOptionProperty 
*/ 

template<typename T>
class IVW_CORE_API TemplatedOptionProperty : public BaseOptionProperty {

public:
    TemplatedOptionProperty(std::string identifier, std::string displayName, std::string value,
                            PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                    PropertySemantics::Type semantics = PropertySemantics::Default);
    /**  
     * \brief Adds a option to the property 
     * 
     * Adds a option to the property and stores it as a pair in the optionVector_ 
     * The option name is the name of the option that will be displayed in the widget. 
     *  
     * @See optionpropertywidgetqt 
     * @param std::string optionName identifier name 
     * @param T optionValue the value of the option 
     */virtual void addOption(std::string optionName,T optionValue);
    /**  
     * \brief Returns the vector of options 
     * 
     *  
     * @return std::vector< std::pair<std::string, T> > <DESCRIBE ME> 
     */virtual std::vector< std::pair<std::string, T> > getOptions();
    /**  
     * \brief returns a vector of keys from the option vector 
     * 
     *  
     * @return std::vector< std::string > <DESCRIBE ME> 
     */std::vector< std::string > getOptionKeys(); 
 
    /**  
     * \brief Set the currently selected option 
     * 
     * <DESCRIBE THE METHOD> 
     *  
     * @param std::string the key to the desiered value 
     */void setSelectedOption( std::string ); 

    int getSelectedOption();
    T getSelectedValue();
    virtual void updateValue( std::string);

private:
    std::vector< std::pair<std::string, T> > optionVector_;
};


template <typename T>
TemplatedOptionProperty<T>::TemplatedOptionProperty(std::string identifier, std::string displayName, std::string value,
                                                    PropertyOwner::InvalidationLevel invalidationLevel,
                                                    PropertySemantics::Type semantics )
    : BaseOptionProperty(identifier, displayName, value, invalidationLevel, semantics)
{}

template<typename T>
void TemplatedOptionProperty<T>::addOption(std::string optionName,T optionValue) {
    optionVector_.push_back(std::make_pair(optionName,optionValue));
}
template<typename T>
std::vector< std::pair<std::string, T> > TemplatedOptionProperty< T >::getOptions() {
    return optionVector_;
}

template<typename T>
std::vector< std::string > TemplatedOptionProperty<T>::getOptionKeys(){
    
    std::vector< std::string > ret;
    size_t size = optionVector_.size();
    for (size_t i=0; i<size;i++) {
        ret.push_back(optionVector_.at(i).first);
    }
    return ret;
}

template<typename T>
int TemplatedOptionProperty<T>::getSelectedOption() {
    std::vector< std::string > tmp = getOptionKeys();
    int size = static_cast<int>(tmp.size());
    for (int i=0; i<size;i++) {
        if(getOptionKeys().at(i)== value_)
            return i;
    }
    return 0;
}

template<typename T>
T TemplatedOptionProperty<T>::getSelectedValue() {
    return optionVector_[getSelectedOption()].second;
}

template<typename T>
void TemplatedOptionProperty<T>::setSelectedOption(std::string tmpStr) {
    set(tmpStr);
}

template<typename T>
void TemplatedOptionProperty<T>::updateValue(std::string tmpStr) {
    set(tmpStr);
}

} // namespace

#endif // IVW_BASEOPTIONPROPERTY_H