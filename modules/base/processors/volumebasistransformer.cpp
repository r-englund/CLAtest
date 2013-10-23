#include "volumebasistransformer.h"
#include "inviwo\core\datastructures\volume\volumeram.h"

namespace inviwo {

ProcessorClassName(VolumeBasisTransformer, "VolumeBasisTransformer"); 
ProcessorCategory(VolumeBasisTransformer, "Volume Operation");
ProcessorCodeState(VolumeBasisTransformer, CODE_STATE_EXPERIMENTAL);

VolumeBasisTransformer::VolumeBasisTransformer()
    : Processor(),
      inport_("volume.in"),
      outport_("volume.out"),
      lengths_("length_", "Lengths", vec3(1.0f), vec3(0.0f), vec3(10.0f)),
      angels_("angles_", "Angles", vec3(90.0f), vec3(0.0f), vec3(180.0f), vec3(1.0f)),
      offset_("offset_", "Offset", vec3(0.0f), vec3(-10.0f), vec3(10.0f)),
      orgBasisAndOffset_(0.0f)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(lengths_);
    addProperty(angels_);
    addProperty(offset_);
}

VolumeBasisTransformer::~VolumeBasisTransformer() {

}

void VolumeBasisTransformer::process() {
    if(inport_.hasData()){
        const Volume* in = inport_.getData();

        if( orgBasisAndOffset_ != in->getBasisAndOffset()){
            orgBasisAndOffset_ = in->getBasisAndOffset();

            vec3 a(glm::transpose(orgBasisAndOffset_)[0]);
            vec3 b(glm::transpose(orgBasisAndOffset_)[1]);
            vec3 c(glm::transpose(orgBasisAndOffset_)[2]);
            vec3 offset(glm::transpose(orgBasisAndOffset_)[3]);

            float alpha = glm::angle(b,c);
            float beta = glm::angle(c,a);
            float gamma = glm::angle(a,b);

            lengths_.set(vec3(glm::length(a),glm::length(b),glm::length(c)));
            angels_.set(vec3(alpha, beta, gamma));
            offset_.set(offset);
        }
        
        // TODO
        // This should be changed to make some kind of shallow copy of the representations
        // and update if the inport changes the representations
        Volume* out;
        if(outport_.hasData()){
            out = outport_.getData();
        }else{
            out = in->clone();
            outport_.setData(out);
        }
        
        float a = lengths_.get()[0];
        float b = lengths_.get()[1];
        float c = lengths_.get()[2];
        vec3 offset = offset_.get();
        float alpha = glm::radians(angels_.get()[0]);
        float beta =  glm::radians(angels_.get()[1]);
        float gamma = glm::radians(angels_.get()[2]);

        float v = std::sqrt(1 - std::cos(alpha)*std::cos(alpha) - std::cos(beta)*std::cos(beta) - std::cos(gamma)*std::cos(gamma)
            - 2*std::cos(alpha)*std::cos(beta)*std::cos(gamma));

        mat4 newBasisAndOffset(
            a,    b*std::cos(gamma), c*std::cos(beta),                                                   offset[0],
            0.0f, b*std::sin(gamma), c*(std::cos(alpha)-std::cos(beta)*std::cos(gamma))/std::sin(gamma), offset[1], 
            0.0f, 0.0f,              c*v/std::sin(gamma),                                                offset[2],
            0.0f, 0.0f,              0.0f,                                                               1.0f
            );
        out->setBasisAndOffset(newBasisAndOffset);
    }
}

void VolumeBasisTransformer::initialize() {
    Processor::initialize();
}

void VolumeBasisTransformer::deinitialize() {
    Processor::deinitialize();
}

} // inviwo namespace
