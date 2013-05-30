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

    virtual std::vector<std::string> getOptionDisplayNames() const = 0;
};


/** class TemplateOptionProperty
*   Template class for option properties
* 
* @see OptionProperties
* @see BaseOptionProperty
*/
template<typename T>
class IVW_CORE_API TemplateOptionProperty : public BaseOptionProperty {

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
     * \brief Adds a option to the property
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

    bool isSelected(T value) const;

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

private:
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
}

template<typename T>
int TemplateOptionProperty<T>::numOptions() const {
    return options_.size();
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
    for (size_t i=0; i<options_.size(); i++) {
        if (options_[i].second == value_)
            return i;
    }
    return 0;
}

template<typename T>
void TemplateOptionProperty<T>::setSelectedOption(int option) {
    set(options_[option].second);
}

template<typename T>
bool TemplateOptionProperty<T>::isSelected(T value) const {
    return (value_ == value);
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
    onChangeCallback_.invoke();
    if (getOwner()) getOwner()->invalidate(getInvalidationLevel());
    updatePropertyWidgets();
}

} // namespace inviwo

#endif // IVW_BASEOPTIONPROPERTY_H