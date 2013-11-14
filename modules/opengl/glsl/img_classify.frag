#include "modules/mod_classification.frag"

uniform sampler2D inport_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_;
    vec4 value = texture2D(inport_, texCoords);
    vec4 color = applyTF(transferFunc_, value);
    FragData0 = color;
}