#include <inviwo/core/properties/simpleraycastingproperty.h>

namespace inviwo {

PropertyClassIdentifier(SimpleRaycastingProperty, "org.inviwo.SimpleRaycastingProperty");

SimpleRaycastingProperty::SimpleRaycastingProperty(
    std::string identifier, std::string displayName,
    PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics semantics)
    : CompositeProperty(identifier, displayName, invalidationLevel, semantics)
    , classificationMode_("classificationMode", "Classification", PropertyOwner::INVALID_RESOURCES)
    , compositingMode_("compositingMode", "Compositing", PropertyOwner::INVALID_RESOURCES)
    , gradientComputationMode_("gradientComputationMode", "Gradient",
    PropertyOwner::INVALID_RESOURCES)
    , samplingRate_("samplingRate", "Sampling rate", 2.0f, 1.0f, 10.0f)
    , isoValue_("isoValue", "Iso value", 0.5f, 0.0f, 1.0f) {

    classificationMode_.addOption("none", "None");
    classificationMode_.addOption("transfer-function", "Transfer function");
    classificationMode_.setSelectedIdentifier("transfer-function");
    classificationMode_.setCurrentStateAsDefault();
    addProperty(classificationMode_);

    compositingMode_.addOption("dvr", "Direct volume rendering");
    compositingMode_.addOption("mip", "Maximum intensity projection");
    compositingMode_.addOption("fhp", "First hit points");
    compositingMode_.addOption("fhn", "First hit normals");
    compositingMode_.addOption("fhnvs", "First hit normals (ViewSpace)");
    compositingMode_.addOption("fhd", "First hit depth");
    compositingMode_.addOption("iso", "Iso surface rendering");
    compositingMode_.addOption("ison", "Iso surface normal rendering");
    compositingMode_.setSelectedIdentifier("dvr");
    compositingMode_.setCurrentStateAsDefault();
    addProperty(compositingMode_);

    gradientComputationMode_.addOption("none", "None");
    gradientComputationMode_.addOption("forward", "Forward differences");
    gradientComputationMode_.addOption("backward", "Backward differences");
    gradientComputationMode_.addOption("central", "Central differences");
    gradientComputationMode_.addOption("central-higher", "Higher order central differences");
    gradientComputationMode_.setSelectedIdentifier("central");
    gradientComputationMode_.setCurrentStateAsDefault();
    addProperty(gradientComputationMode_);

    addProperty(samplingRate_);
    addProperty(isoValue_);

}

}  // namespace
