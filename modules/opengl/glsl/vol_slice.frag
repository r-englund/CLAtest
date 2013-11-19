#include "modules/mod_sampler2d.frag"
#include "modules/mod_sampler3d.frag"
#include "modules/mod_classification.frag"

uniform VOLUME_TYPE volume_;
uniform VOLUME_PARAMETERS volumeParameters_;

uniform vec2 dimension_;
uniform float sliceNum_;

void main() {
    vec2 texC = gl_FragCoord.xy * screenDimRCP_;
    vec4 voxel = getVoxel(volume_, volumeParameters_, vec3(coordPlanePermute(texC.x, texC.y, sliceNum_)));
    vec4 color = applyTF(transferFunc_, voxel);
    FragData0 = color;
}