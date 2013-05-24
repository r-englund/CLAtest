uniform vec2 screenDim_;
uniform vec2 screenDimRCP_;

struct TEXTURE_PARAMETERS {
    vec2 dimensions_;
    vec2 dimensionsRCP_;
};

#define SAMPLER2D_TYPE sampler2D

vec4 textureLookup2Dscreen(SAMPLER2D_TYPE texture, TEXTURE_PARAMETERS textureParams, vec2 samplePos) {
    return texture2D(texture, samplePos);
}