#include "diffuselightsourceprocessor.h"

namespace inviwo {

ProcessorClassName(DiffuseLightSourceProcessor, "Diffuse light source"); 
ProcessorCategory(DiffuseLightSourceProcessor, "Light source");
ProcessorCodeState(DiffuseLightSourceProcessor, CODE_STATE_EXPERIMENTAL);


DiffuseLightSourceProcessor::DiffuseLightSourceProcessor()
    : Processor(),
      outport_("DiffuseLightSource"),
      lightPowerProp_("lightPower", "Light power (%)", 50.f, 0.f, 100.f),
      lightDiffuse_("lightDiffuse", "Color", vec3(1.0f, 1.0f, 1.0f)),
      lightPosition_("lightPosition", "Light Source Position", vec3(1.f, 0.65f, 0.65f), vec3(-1.f), vec3(1.f)),
      lightSize_("lightSize", "Light size", vec2(1.5f, 1.5f), vec2(0.0f, 0.0f), vec2(3.0f, 3.0f))
{


    addProperty(lightPosition_);
    addProperty(lightDiffuse_);
    addProperty(lightPowerProp_);
    addProperty(lightSize_);

    addPort(outport_);
    


    // assign lighting properties to property group
    lightPosition_.setGroupID("lighting");
    lightDiffuse_.setGroupID("lighting");
    lightPowerProp_.setGroupID("lighting");
    lightSize_.setGroupID("lighting");

    Property::setGroupDisplayName("lighting", "Light Parameters");

    lightPosition_.setSemantics(PropertySemantics::LightPosition);
    lightDiffuse_.setSemantics(PropertySemantics::Color);




}

void DiffuseLightSourceProcessor::process() {
   updateLightSource(&lightSource_);
   outport_.setData(&lightSource_, false);

}



void DiffuseLightSourceProcessor::updateLightSource(DiffuseLight* lightSource) {

    vec3 lightPos = vec3(0.5f, 0.5f, 0.5f) + lightPosition_.get();
    vec3 dir = glm::normalize(vec3(0.5f, 0.5f, 0.5f)-lightPos);

    vec3 A = vec3(0,0,1);
    vec3 B = dir;//B(0,1,0);
    float angle = acos(glm::dot(A,B));
    vec3 rotationAxis = glm::normalize(glm::cross(A, B));

#ifndef GLM_FORCE_RADIANS
    angle = glm::degrees(angle);
#endif // GLM_FORCE_RADIANS

   
    mat4 transformationMatrix = glm::translate(lightPos)*glm::rotate(angle, rotationAxis);

    lightSource->setObjectToTexture(transformationMatrix);
    lightSource->setSize(lightSize_.get());
    lightSource->setIntensity(lightPowerProp_.get()*lightDiffuse_.get());
    lightSource->setLightDirection(dir);
}

} // namespace
