#ifndef IVW_BASEOPTIONPROPERTY_H
#define IVW_BASEOPTIONPROPERTY_H


#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

/** class BaseOptionProperty
*   Base class for the option properties
* 
* @see TemplateOptionProperty
*/
class IVW_CORE_API BaseOptionProperty : public Property {
public:

    BaseOptionProperty(std::string identifier, std::string displayName,
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics=PropertySemantics::Default)
        : Property(identifier, displayName, invalidationLevel, semantics)
    {}

    virtual int numOptions() const = 0;
    virtual int getSelectedOption() const = 0;
    virtual void setSelectedOption(int option) = 0;
    virtual void clearOptions() = 0;

    virtual std::vector<std::string> getOptionDisplayNames() const = 0;
    virtual std::string getClassName()  const { return "BaseOptionProperty"; }
};


/** class TemplateOptionProperty
*   Template class for option properties
* 
* @see OptionProperties
* @see BaseOptionProperty
*/
template<typename T>
class TemplateOptionProperty : public BaseOptionProperty {

//FIXME: use struct instead of std::pair combination for options
/*
template <typename T> 
struct Option { 
    std::string identifier;
    std::string displayName;
    T value;
};
*/

public:
    TemplateOptionProperty(std::string identifier, std::string displayName,
                           PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                           PropertySemantics::Type semantics=PropertySemantics::Default);
    TemplateOptionProperty(std::string identifier, std::string displayName, T value,
                           PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                           PropertySemantics::Type semantics=PropertySemantics::Default);


    /** 
     * \brief Adds an option to the property
     *
     * Adds a option to the property and stores it as a pair in the options_
     * The option name is the name of the option that will be displayed in the widget.
     * 
     * @param std::string identifier identifier name
     * @param T value the value of the option
     */
    virtual void addOption(std::string identifier, std::string displayName, T value);


    /** 
     * \brief returns a vector of keys from the option vector
     *
     * 
     * @return std::vector< std::string > <DESCRIBE ME>
     */
    virtual std::vector<std::string> getOptionDisplayNames() const;

    virtual int numOptions() const;

    /** 
     * \brief Returns the current selected option of the property
     *
     * 
     * @return int the index of the selected option
     */
    virtual int getSelectedOption() const;


    virtual void setSelectedOption(int option);

    void getAllOptions(std::vector<T>& allValues) const;
    
    virtual void clearOptions();

    bool isSelected(std::string identifier) const;

    /** 
     * \brief returns the value of the currently selected option
     *
     * 
     * @return T <DESCRIBE ME>
     */
    T getValue() const; // deprecated
    T get() const;

    /** 
     * \brief Set the currently selected option
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param std::string the key to the desiered value
     */
    void set(T value);

    void setToID(std::string identifier);
    void selectByKey(std::string identifier);
    
    /** 
     * \brief Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.         
     * 
     * @return int VariantType as integer
     */
    virtual int getVariantType() {
        return Variant::VariantTypeString;
    }

protected:
    T value_;
    std::vector< std::pair<std::pair<std::string, std::string>, T> > options_;
};


template <typename T>
TemplateOptionProperty<T>::TemplateOptionProperty(std::string identifier, std::string displayName,
                                                    PropertyOwner::InvalidationLevel invalidationLevel,
                                                    PropertySemantics::Type semantics )
    : BaseOptionProperty(identifier, displayName, invalidationLevel, semantics)
{}

template <typename T>
TemplateOptionProperty<T>::TemplateOptionProperty(std::string identifier, std::string displayName, T value,
                                                    PropertyOwner::InvalidationLevel invalidationLevel,
                                                    PropertySemantics::Type semantics )
    : BaseOptionProperty(identifier, displayName, invalidationLevel, semantics),
      value_(value)
{}

template<typename T>
void TemplateOptionProperty<T>::addOption(std::string optionIdentifier, std::string optionDisplayName, T optionValue) {
    options_.push_back(std::make_pair(std::make_pair(optionIdentifier, optionDisplayName), optionValue));
    // in case we add the first option, we also select it
    if (options_.size() == 1) setSelectedOption(0);
}

template<typename T>
int TemplateOptionProperty<T>::numOptions() const {
    return static_cast<int>(options_.size());
}

template<typename T>
std::vector<std::string> TemplateOptionProperty<T>::getOptionDisplayNames() const {
    std::vector<std::string> result;
    for (size_t i=0; i<options_.size(); i++) {
        result.push_back(options_[i].first.second);
    }
    return result;
}

template<typename T>
int TemplateOptionProperty<T>::getSelectedOption() const {
    for (int i=0; i<static_cast<int>(options_.size()); i++) {
        if (options_[i].second == value_)
            return i;
    }
    return 0;
}

template<typename T>
void TemplateOptionProperty<T>::getAllOptions(std::vector<T>& allValues) const {
    for (int i=0; i<static_cast<int>(options_.size()); i++) {
       allValues.push_back(options_[i].second);
    }
}


template<typename T>
void TemplateOptionProperty<T>::setSelectedOption(int option) {
    set(options_[option].second);
}

template<typename T>
void TemplateOptionProperty<T>::clearOptions() {
    options_.clear();
}

template<typename T>
bool TemplateOptionProperty<T>::isSelected(std::string identifier) const {
    for (size_t i=0; i<options_.size(); i++)
        if (options_[i].first.first == identifier)
            return (options_[i].second == value_);
    LogWarn("Querying non-existent option.");
    return false;
}

template<typename T>
T TemplateOptionProperty<T>::getValue() const {
    ivwDeprecatedMethod("get");
    return get();
}

template<typename T>
T TemplateOptionProperty<T>::get() const {
    return value_;
}

template<typename T>
void TemplateOptionProperty<T>::set(T value) {
    value_ = value;
    propertyModified();
}

template<typename T>
void TemplateOptionProperty<T>::setToID(std::string identifier) {
    for (size_t i=0; i<options_.size(); i++)
        if (options_[i].first.second == identifier) {
            set(options_[i].second);
            return;
        }
}

template<typename T>
void TemplateOptionProperty<T>::selectByKey(std::string identifier) {
    ivwDeprecatedMethod("setToID");
    setToID(identifier);
}

} // namespace inviwo

#endif // IVW_BASEOPTIONPROPERTY_H