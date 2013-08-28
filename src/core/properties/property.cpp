#include <inviwo/core/properties/property.h>

namespace inviwo {

    std::map<std::string,std::string> Property::groupDisplayNames_;

Property::Property(std::string identifier, std::string displayName, PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics)
: identifier_(identifier),
  displayName_(displayName),
  invalidationLevel_(invalidationLevel),
  semantics_(semantics),
  owner_(0),
  groupID_(""),
  VoidObservable(),
  visibilityMode_(PropertyVisibility::APPLICATION)
{}

Property::Property()
    : identifier_(""),
      displayName_("")
{}

std::string Property::getIdentifier() const {
    return identifier_;
}

void Property::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
}

std::string Property::getDisplayName() const {
    return displayName_;
}

void Property::setDisplayName(const std::string& displayName) {
    displayName_ = displayName;
}

void Property::setSemantics( const PropertySemantics::Type& semantics) {
    semantics_ = semantics;
}

inviwo::PropertySemantics::Type Property::getSemantics() const {
    return semantics_;
}

PropertyOwner* Property::getOwner() {
    return owner_;
}

void Property::setOwner(PropertyOwner* owner) {
    owner_ = owner;
}

void Property::registerPropertyWidget(PropertyWidget* propertyWidget) {
    propertyWidgets_.push_back(propertyWidget);
    updateVisibility();
}

void Property::updatePropertyWidgets() {
    for (size_t i=0; i<propertyWidgets_.size(); i++)
        propertyWidgets_[i]->updateFromProperty();
}

void Property::invalidate() {
    invalid_ = true;
}

void Property::setValid() {
    invalid_ = false;
}

bool Property::isValid() {
    return (invalid_ == false);
}

Variant Property::getVariant() {
    return Variant(getVariantType());
}

void Property::setVariant(const Variant&) {}

int Property::getVariantType() {
    return Variant::VariantTypeInvalid;
}

void Property::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
    s.serialize("identifier", identifier_, true);
    s.serialize("displayName", displayName_, true);
    s.serialize("visibilityMode", visibilityMode_);
}

void Property::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("identifier", identifier_, true);
    d.deserialize("displayName", displayName_, true);

    int visibilityMode;
    d.deserialize("visibilityMode", visibilityMode);
    setVisibility(static_cast<PropertyVisibility::VisibilityMode>(visibilityMode));
}

bool Property::operator==( Property* prop){
    if (this->getIdentifier()==prop->getIdentifier()) {
        return true;
    }
    return false;
}


void Property::setVisibility( PropertyVisibility::VisibilityMode visibilityMode ){
   visibilityMode_ = visibilityMode;
   updateVisibility();
}

void Property::setGroupDisplayName( std::string groupID,std::string groupDisplayName ){
        Property::groupDisplayNames_.insert(std::pair<std::string,std::string>(groupID,groupDisplayName));    
}

std::string Property::getGroupDisplayName(){
    return groupDisplayNames_[groupID_];
}

void Property::notify(){
    updateVisibility();
}

void Property::updateVisibility(){

    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    if (dynamic_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))) {
        PropertyVisibility::VisibilityMode inviwoAppViewMode = 
            static_cast<PropertyVisibility::VisibilityMode>(dynamic_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());

        addObservation(dynamic_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode")));

        if (visibilityMode_== PropertyVisibility::DEVELOPMENT && inviwoAppViewMode == PropertyVisibility::APPLICATION) {
            for (size_t i=0; i<propertyWidgets_.size(); i++)
                propertyWidgets_[i]->hideWidget();
        }
        if (inviwoAppViewMode == PropertyVisibility::DEVELOPMENT) {
            for (size_t i=0; i<propertyWidgets_.size(); i++)
                propertyWidgets_[i]->showWidget();
        }
    }
    if (visibilityMode_ == PropertyVisibility::INVISIBLE) {
        for (size_t i=0; i<propertyWidgets_.size(); i++)
            propertyWidgets_[i]->hideWidget();
    }
}


} // namespace
