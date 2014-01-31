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

#include <inviwo/core/properties/optionproperties.h>

namespace inviwo {

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : TemplateOptionProperty<int>(identifier, displayName, 0, invalidationLevel, semantics)
{}

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName, int value,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : TemplateOptionProperty<int>(identifier, displayName, value, invalidationLevel, semantics)
{}

Variant OptionPropertyInt::getVariant() {
    std::stringstream ss;
    ss << getSelectedOption() << std::endl;
    for (size_t i=0; i<options_.size(); i++) {
        ss << options_[i].first.first  << std::endl;
        ss << options_[i].first.second  << std::endl;
    }    
    return Variant(ss.str());
}

void OptionPropertyInt::setVariant(const Variant& inVariant) {
    std::string textLine;    
    std::istringstream ss(inVariant.getString());
    int seletctedOption=0;
    getline(ss, textLine);
    seletctedOption = atoi(textLine.c_str());
    options_.clear();
    while(!ss.eof()) { 
        std::string identifier, displayName, valStr;
        getline(ss, identifier);        
        getline(ss, displayName);  
        getline(ss, valStr);
        int val = atoi(valStr.c_str());     
        addOption(identifier, displayName, val);
    }
    setSelectedOption(seletctedOption);
}

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName,
                                         PropertyOwner::InvalidationLevel invalidationLevel,
                                         PropertySemantics semantics )
    : TemplateOptionProperty<float>(identifier, displayName, 0.0f, invalidationLevel, semantics)
{}

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName, float value,
                                         PropertyOwner::InvalidationLevel invalidationLevel,
                                         PropertySemantics semantics )
    : TemplateOptionProperty<float>(identifier, displayName, value, invalidationLevel, semantics)
{}

Variant OptionPropertyFloat::getVariant() {
    std::stringstream ss;
    ss << getSelectedOption() << std::endl;
    for (size_t i=0; i<options_.size(); i++) {
        ss << options_[i].first.first << std::endl;
        ss << options_[i].first.second << std::endl;
        ss << options_[i].second << std::endl;
    }    
    return Variant(ss.str());
}

void OptionPropertyFloat::setVariant(const Variant& inVariant) {
    std::string textLine;    
    std::istringstream ss(inVariant.getString());
    int seletctedOption=0;
    getline(ss, textLine);
    seletctedOption = atoi(textLine.c_str());
    options_.clear();
    while(!ss.eof()) { 
        std::string identifier, displayName, valStr;
        getline(ss, identifier);        
        getline(ss, displayName);  
        getline(ss, valStr);
        float val = static_cast<float>(atof(valStr.c_str()));
        addOption(identifier, displayName, val);
    }
    setSelectedOption(seletctedOption);
}

OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics semantics)
    : TemplateOptionProperty<double>(identifier, displayName, 0.0, invalidationLevel, semantics)
{}

OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName, double value,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics semantics)
    : TemplateOptionProperty<double>(identifier, displayName, value, invalidationLevel, semantics)
{}

Variant OptionPropertyDouble::getVariant() {
    std::stringstream ss;
    ss << getSelectedOption() << std::endl;
    for (size_t i=0; i<options_.size(); i++) {
        ss << options_[i].first.first << std::endl;
        ss << options_[i].first.second << std::endl;
        ss << options_[i].second << std::endl;
    }    
    return Variant(ss.str());
}

void OptionPropertyDouble::setVariant(const Variant& inVariant) {
    std::string textLine;    
    std::istringstream ss(inVariant.getString());
    int seletctedOption=0;
    getline(ss, textLine);
    seletctedOption = atoi(textLine.c_str());
    options_.clear();
    while(!ss.eof()) { 
        std::string identifier, displayName, valStr;
        getline(ss, identifier);        
        getline(ss, displayName);  
        getline(ss, valStr);
        double val = atof(valStr.c_str());
        addOption(identifier, displayName, val);
    }
    setSelectedOption(seletctedOption);
}

OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics semantics)
                                           : TemplateOptionProperty<std::string>(identifier, displayName, identifier, invalidationLevel, semantics)
{}

OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName, std::string value,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics semantics)
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
    ss << getSelectedOption() << std::endl;
    for (size_t i=0; i<options_.size(); i++) {
        ss << options_[i].first.first << std::endl;
        ss << options_[i].first.second << std::endl;
        ss << options_[i].second << std::endl;
    }    
    return Variant(ss.str());
}

void OptionPropertyString::setVariant(const Variant& inVariant) {
    std::string textLine;    
    std::istringstream ss(inVariant.getString());
    int seletctedOption=0;
    getline(ss, textLine);
    seletctedOption = atoi(textLine.c_str());
    options_.clear();
    while(!ss.eof()) { 
        std::string identifier, displayName, val;        
        getline(ss, identifier);
        getline(ss, displayName);
        getline(ss, val);        
       addOption(identifier, displayName, val);
    }    
    setSelectedOption(seletctedOption);
    
}

} // namespace
