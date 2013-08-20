#include <inviwo/core/properties/optionproperties.h>

namespace inviwo {

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics)
    : TemplateOptionProperty<int>(identifier, displayName, 0, invalidationLevel, semantics)
{}

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName, int value,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics)
    : TemplateOptionProperty<int>(identifier, displayName, value, invalidationLevel, semantics)
{}

Variant OptionPropertyInt::getVariant() {
    std::stringstream ss;
    ss << " " << getSelectedOption() ;
    for (size_t i=0; i<options_.size(); i++) {
        ss << " " << options_[i].first.first << " " << options_[i].first.second;
    }    
    return Variant(ss.str());
}

void OptionPropertyInt::setVariant(const Variant& inVariant) {
    std::istringstream ss(inVariant.getString());
    int seletctedOption=0;
    ss >> seletctedOption ;
    options_.clear();
    while(!ss.eof()) {
        std::string identifier, displayName;
        int val=0;
        ss >> identifier;
        ss >> displayName;            
        addOption(identifier, displayName, val);
    }
    setSelectedOption(seletctedOption);
}

void OptionPropertyInt::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyInt::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    int value;
    d.deserialize("value", value);
    set(value);
}

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName,
                                         PropertyOwner::InvalidationLevel invalidationLevel,
                                         PropertySemantics::Type semantics )
    : TemplateOptionProperty<float>(identifier, displayName, 0.0f, invalidationLevel, semantics)
{}

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName, float value,
                                         PropertyOwner::InvalidationLevel invalidationLevel,
                                         PropertySemantics::Type semantics )
    : TemplateOptionProperty<float>(identifier, displayName, value, invalidationLevel, semantics)
{}

Variant OptionPropertyFloat::getVariant() {
    std::stringstream ss;
    ss << " " << getSelectedOption() ;
    for (size_t i=0; i<options_.size(); i++) {
        ss << " " << options_[i].first.first << " " << options_[i].first.second <<" " << options_[i].second;
    }    
    return Variant(ss.str());
}

void OptionPropertyFloat::setVariant(const Variant& inVariant) {
    std::istringstream ss(inVariant.getString());
    int seletctedOption=0;
    ss >> seletctedOption ;
    options_.clear();
    while(!ss.eof()) {
        std::string identifier, displayName;
        float val=0.0f;
        ss >> identifier;
        ss >> displayName;            
        addOption(identifier, displayName, val);
    }
    setSelectedOption(seletctedOption);
}

void OptionPropertyFloat::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyFloat::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    float value;
    d.deserialize("value", value);
    set(value);
}


OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics)
    : TemplateOptionProperty<double>(identifier, displayName, 0.0, invalidationLevel, semantics)
{}

OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName, double value,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics)
    : TemplateOptionProperty<double>(identifier, displayName, value, invalidationLevel, semantics)
{}

Variant OptionPropertyDouble::getVariant() {
    std::stringstream ss;
    ss << " " << getSelectedOption() ;
    for (size_t i=0; i<options_.size(); i++) {
        ss << " " << options_[i].first.first << " " << options_[i].first.second <<" " << options_[i].second;
    }    
    return Variant(ss.str());
}

void OptionPropertyDouble::setVariant(const Variant& inVariant) {
    std::istringstream ss(inVariant.getString());
    int seletctedOption=0;
    ss >> seletctedOption ;
    options_.clear();
    while(!ss.eof()) {
        std::string identifier, displayName;
        double val=0.0;
        ss >> identifier;
        ss >> displayName;            
        addOption(identifier, displayName, val);
    }
    setSelectedOption(seletctedOption);
}

void OptionPropertyDouble::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyDouble::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    double value;
    d.deserialize("value", value);
    set(value);
}


OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics)
                                           : TemplateOptionProperty<std::string>(identifier, displayName, identifier, invalidationLevel, semantics)
{}

OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName, std::string value,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics)
                                           : TemplateOptionProperty<std::string>(identifier, displayName, value, invalidationLevel, semantics)
{}

void OptionPropertyString::addOption(std::string identifier, std::string displayName) {
    TemplateOptionProperty<std::string>::addOption(identifier, displayName, identifier);
}

void OptionPropertyString::addOption(std::string identifier, std::string displayName, std::string value) {
    TemplateOptionProperty<std::string>::addOption(identifier, displayName, value);
}

Variant OptionPropertyString::getVariant() {
    std::stringstream ss;
    ss << " " << getSelectedOption() ;
    for (size_t i=0; i<options_.size(); i++) {
        ss << " " << options_[i].first.first << " " << options_[i].first.second <<" " << options_[i].second;
    }    
    return Variant(ss.str());
}

void OptionPropertyString::setVariant(const Variant& inVariant) {
    std::istringstream ss(inVariant.getString());
    int seletctedOption=0;
    ss >> seletctedOption ;
    options_.clear();
    while(!ss.eof()) {
        std::string identifier, displayName, val;        
        ss >> identifier;
        ss >> displayName;  
        ss >> val;
       addOption(identifier, displayName, val);
    }    
    setSelectedOption(seletctedOption);
    
}

void OptionPropertyString::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyString::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string value;
    d.deserialize("value", value);
    set(value);
}

} // namespace
