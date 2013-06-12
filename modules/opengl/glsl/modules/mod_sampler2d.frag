uniform vec2 screenDim_;
uniform vec2 screenDimRCP_;

#define TEXTURE_TYPE sampler2D

struct TEXTURE_PARAMETERS {
    vec2 dimensions_;
    vec2 dimensionsRCP_;
};


vec4 textureLookup2D(TEXTURE_TYPE texture, TEXTURE_PARAMETERS textureParams, vec2 samplePos) {
    //deprecated
    return texture2D(texture, samplePos);
}

vec4 textureLookup2Dnormalized(TEXTURE_TYPE texture, TEXTURE_PARAMETERS textureParams, vec2 samplePos) {
    return texture2D(texture, samplePos);
}

vec4 textureLookup2Dscreen(TEXTURE_TYPE texture, TEXTURE_PARAMETERS textureParams, vec2 samplePos) {
    return texture2D(texture, samplePos*screenDimRCP_);
}