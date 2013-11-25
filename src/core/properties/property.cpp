#include <inviwo/core/properties/property.h>
#include <inviwo/core/util/variant.h>
#include <inviwo/core/common/inviwoapplication.h>

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
  visibilityMode_(PropertyVisibility::APPLICATION),
  readOnly_(false),
  propertyModified_(false)
{}

Property::Property()
    : identifier_(""),
      displayName_(""),
      propertyModified_(false)
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
    if(this->visibilityMode_ == PropertyVisibility::INVISIBLE)
        updateVisibility();
}

void Property::updatePropertyWidgets() {
    for (size_t i=0; i<propertyWidgets_.size(); i++)
        if (propertyWidgets_[i] != 0)
            propertyWidgets_[i]->updateFromProperty();
}

void Property::propertyModified() { 
    onChangeCallback_.invoke();
    setPropertyModified(true); 
    //FIXME: if set() is called before addProperty(), getOwner() will be 0 ( case for option properties )    
    if (getOwner()) getOwner()->invalidate(getInvalidationLevel());    
    updatePropertyWidgets();
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
}

void Property::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("identifier", identifier_, true);
    d.deserialize("displayName", displayName_, true);
}

bool Property::operator==(const Property& prop){
	// TODO: this is not an equal operator, change to isSameType or similar
    if (this->getClassName()==prop.getClassName()) return true;
	else return false;
}

void Property::setGroupDisplayName(std::string groupID, std::string groupDisplayName) {
	Property::groupDisplayNames_.insert(std::pair<std::string,std::string>(groupID, groupDisplayName));
}

std::string Property::getGroupDisplayName(){
    return groupDisplayNames_[groupID_];
}

void Property::setVisibility(PropertyVisibility::VisibilityMode visibilityMode) {
    this->visibilityMode_ = visibilityMode;
    updateVisibility();
}

void Property::updateVisibility() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode appMode = inviwoApp->getPropertyVisibilityMode();

    if (visibilityMode_ == PropertyVisibility::INVISIBLE) {
        for (size_t i=0; i<propertyWidgets_.size(); i++){
            propertyWidgets_[i]->hideWidget();
        }
    }
    if (visibilityMode_ == PropertyVisibility::APPLICATION) {
        for (size_t i=0; i<propertyWidgets_.size(); i++){
            propertyWidgets_[i]->showWidget();
        }
    }
    else if (visibilityMode_ == PropertyVisibility::DEVELOPMENT && appMode == PropertyVisibility::DEVELOPMENT) {
        for (size_t i=0; i<propertyWidgets_.size(); i++){
            propertyWidgets_[i]->showWidget();
        }
    }
    else if (visibilityMode_ == PropertyVisibility::DEVELOPMENT && appMode == PropertyVisibility::APPLICATION ) {
        for (size_t i=0; i<propertyWidgets_.size(); i++)
            propertyWidgets_[i]->hideWidget();
    }

}

void Property::setVisible( bool val ) {
    if (val)
        setVisibility(PropertyVisibility::APPLICATION);
    else
        setVisibility(PropertyVisibility::INVISIBLE);
}

} // namespace
